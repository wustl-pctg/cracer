#include <iostream>

/** @todo clang won't compile the gcc 4.4.7 mutex header, so I'll have
    to write my own.
 */
//#include <mutex>
#include <pthread.h>

#include <internal/abi.h>
#include <cilk/batcher.h>
#include "stack.h"
#include "tsan.cpp"
#include "../om/om.c"

typedef enum {
  TOOL = 0,
  TOOL_INSERT,
  OM_INSERT,
  OM_RELABEL,
  SHADOW_STACK_MANIP,
  HEAVY_ARRAY_MANIP,
  BLIST_SPLIT,
  OM_UPDATE_SIZES,
  OM_REBALANCE,
  OM_BUILD_ARRAY_OF_LEAVES,
  OM_REBUILD,
  OM_REBUILD_MALLOC,
  OM_MALLOC,
  NUM_INTERVAL_TYPES,
} interval_types;

#ifdef DEBUG
#define RD_DEBUG(...) printf(__VA_ARGS__)
#else
#define RD_DEBUG(...)
#endif

#ifdef STATS
#include <time.h>
double g_timing_events[NUM_INTERVAL_TYPES];
struct timespec g_timing_event_starts[NUM_INTERVAL_TYPES];
struct timespec g_timing_event_ends[NUM_INTERVAL_TYPES];
struct timespec EMPTY_TIME_POINT;
#define ZERO_DURATION 0
#define INTERVAL_CAST(x) x
#define RDTOOL_INTERVAL_BEGIN(i) clock_gettime(CLOCK_REALTIME, &g_timing_event_starts[(i)])
#define RDTOOL_INTERVAL_END(i) clock_gettime(CLOCK_REALTIME, &g_timing_event_ends[(i)]); \
  g_timing_events[(i)] += (g_timing_event_ends[(i)].tv_sec - g_timing_event_starts[(i)].tv_sec) * 1000.0 + \
    (g_timing_event_ends[(i)].tv_nsec - g_timing_event_starts[(i)].tv_nsec) / 1000000.0; \
  g_timing_event_starts[(i)].tv_sec = 0; \
  g_timing_event_starts[(i)].tv_nsec = 0;
#else // no stats
#define RDTOOL_INTERVAL_BEGIN(i)
#define RDTOOL_INTERVAL_END(i)
#endif // ifdef STATS

#if STATS > 0
unsigned long g_num_relabels = 0;
unsigned long g_num_inserts = 0;
#endif

#ifdef STATS
#define RD_STATS(...) __VA_ARGS__
#else
#define RD_STATS(...)
#endif

int g_tool_init = 0;

om* g_english;
om* g_hebrew;
__thread __cilkrts_worker* t_worker;
pthread_spinlock_t g_relabel_mutex;
pthread_spinlock_t* g_worker_mutexes;

typedef struct FrameData_s {
  om_node* current_english;
  om_node* current_hebrew;
  om_node* cont_english;
  om_node* cont_hebrew;
  om_node* sync_english;
  om_node* sync_hebrew;
  size_t enter_counter;
  size_t spawn_counter;
} FrameData_t;

AtomicStack_t<FrameData_t>* frames;

/// @todo A different design would be to have each OM itself contain a
/// list of heavy nodes to rebalance.
AtomicStack_t<tl_node*> heavy_english;
AtomicStack_t<tl_node*> heavy_hebrew;

#define HALF_BITS ((sizeof(label_t) * 8) / 2)

void init_strand(__cilkrts_worker* w, FrameData_t* init)
{
  t_worker = w;
  assert(frames[w->self].empty());
  frames[w->self].push();
  FrameData_t* f = frames[w->self].head();
  if (init) {
    assert(!init->current_english);
    assert(!init->current_hebrew);
    assert(init->cont_english);
    assert(init->cont_hebrew);
    assert(init->sync_english);
    assert(init->sync_hebrew);
    f->current_english = init->cont_english;
    f->current_hebrew = init->cont_hebrew;
    f->sync_english = init->sync_english;
    f->sync_hebrew = init->sync_hebrew;
    f->cont_english = NULL;
    f->cont_hebrew = NULL;
    f->enter_counter = init->enter_counter;
    f->spawn_counter = init->spawn_counter;
  }  else {
    f->enter_counter = 0;
    f->spawn_counter = 0;
    f->current_english = om_insert_initial(g_english);
    f->current_hebrew = om_insert_initial(g_hebrew);
    RD_STATS(__sync_fetch_and_add(&g_num_inserts, 2));
    f->cont_english = NULL;
    f->cont_hebrew = NULL;
    f->sync_english = NULL;
    f->sync_hebrew = NULL;
  }
}

// Brian Kernighan's algorithm
size_t count_set_bits(label_t label)
{
  size_t count = 0;
  while (label) {
    label &= (label - 1);
    count++;
  }
  return count;
}

void join_batch()
{
  RD_DEBUG("Worker %i joining a relabel operation.\n", t_worker->self);
}

void start_batch(om* ds, AtomicStack_t<tl_node*>& heavy_nodes)
{
  RD_STATS(g_num_relabels++);
  RD_DEBUG("Begin relabel %zu.\n", g_num_relabels)

  om_relabel(ds, heavy_nodes.at(0), heavy_nodes.size());
  heavy_nodes.reset();

  for (int i = 0; i < __cilkrts_get_nworkers(); ++i) {
    pthread_spin_unlock(&g_worker_mutexes[i]);
  }
  pthread_spin_unlock(&g_relabel_mutex);
}

bool trylock_all(pthread_spinlock_t* muts, size_t num)
{
  if (pthread_spin_trylock(&g_relabel_mutex) != 0) return false;
  for (int i = 0; i < num; ++i) {
    pthread_spin_lock(&muts[i]);
  }
  return true;
}

om_node* insert_or_relabel(__cilkrts_worker* w, om* ds,
                           AtomicStack_t<tl_node*>& heavy_nodes, om_node* base)
{
  pthread_spinlock_t* mut = &g_worker_mutexes[w->self];
  while (pthread_spin_trylock(mut) != 0) {
    join_batch();
  }

  om_node* n = om_insert(ds, base);
  while (!n) {
    if (!base->list->half_full) {
      RD_DEBUG("Could not fit into list of size %zu, not half full.\n", base->list->size);
      heavy_nodes.push();
      *heavy_nodes.head() = om_get_tl(base);
    }

    pthread_spin_unlock(mut);
    if (trylock_all(g_worker_mutexes, __cilkrts_get_nworkers()))
      start_batch(ds, heavy_nodes);
    else
      join_batch();
    
    while (pthread_spin_trylock(&g_worker_mutexes[w->self]) != 0) {
      join_batch();
    }
    n = om_insert(ds, base);
  }
  if (!n->list->half_full
      && count_set_bits(n->label) >= HALF_BITS) {
    n->list->half_full = 1;
    RD_DEBUG("List marked as half full with %zu items.\n", n->list->size);
    heavy_nodes.push();
    *heavy_nodes.head() = om_get_tl(n);
  }
  pthread_spin_unlock(mut);
  __sync_fetch_and_add(&g_num_inserts, 1);
  return n;
}

extern "C" void cilk_tool_init(void) 
{
#if STATS > 1
  EMPTY_TIME_POINT.tv_sec = 0; EMPTY_TIME_POINT.tv_nsec = 0;
  for (int i = 0; i < NUM_INTERVAL_TYPES; ++i) {
    g_timing_events[i] = INTERVAL_CAST(ZERO_DURATION);
    g_timing_event_starts[i].tv_sec = EMPTY_TIME_POINT.tv_sec;
    g_timing_event_starts[i].tv_nsec = EMPTY_TIME_POINT.tv_nsec;
    g_timing_event_ends[i].tv_sec = EMPTY_TIME_POINT.tv_sec;
    g_timing_event_ends[i].tv_nsec = EMPTY_TIME_POINT.tv_nsec;
  }
#endif
  g_english = om_new();
  g_hebrew = om_new();
  int p = __cilkrts_get_nworkers();
  frames = new AtomicStack_t<FrameData_t>[p];
  g_worker_mutexes = new pthread_spinlock_t[p];
  for (int i = 0; i < p; ++i) {
    pthread_spin_init(&g_worker_mutexes[i], PTHREAD_PROCESS_PRIVATE);
  }
  pthread_spin_init(&g_relabel_mutex, PTHREAD_PROCESS_PRIVATE);
  g_tool_init = 1;
}

extern "C" void cilk_tool_print(void)
{
#if STATS > 1
  for (int i = 0; i < NUM_INTERVAL_TYPES; ++i) {
    assert(g_timing_event_starts[i].tv_sec == EMPTY_TIME_POINT.tv_sec);
    assert(g_timing_event_starts[i].tv_nsec == EMPTY_TIME_POINT.tv_nsec);
    std::cout << "\t\t" << g_timing_events[i];
  }
  std::cout << std::endl;
#endif
#if STATS > 0  
  std::cout << "Num relabels: " << g_num_relabels << std::endl;
  std::cout << "Num inserts: " << g_num_inserts << std::endl;
#endif
}

extern "C" void cilk_tool_destroy(void) 
{
  cilk_tool_print();
  om_free(g_english);
  om_free(g_hebrew);
  delete[] frames;

  for (int i = 0; i < __cilkrts_get_nworkers(); ++i) {
    pthread_spin_destroy(&g_worker_mutexes[i]);
  }
  pthread_spin_destroy(&g_relabel_mutex);

  delete[] g_worker_mutexes;
}

extern "C" void cilk_steal_success(__cilkrts_worker* w, __cilkrts_worker* victim,
				  __cilkrts_stack_frame* sf)
{
  RD_DEBUG("Worker %i stole from %i\n", w->self, victim->self);
  frames[w->self].reset();
  FrameData_t* loot = (FrameData_t*)malloc(sizeof(FrameData_t));
  frames[victim->self].steal_top(loot);
  init_strand(w, loot);
  free(loot);
}

extern "C" void cilk_tool_c_function_enter(void* this_fn, void* rip){ }
extern "C" void cilk_tool_c_function_leave(void* rip) { }
extern "C" void cilk_enter_begin(__cilkrts_stack_frame* sf, void* this_fn, void* rip)
{ 
}

extern "C" void cilk_enter_helper_begin(__cilkrts_stack_frame* sf, void* this_fn, void* rip)
{
  if (!t_worker) {
    __cilkrts_worker* w = __cilkrts_get_tls_worker();
    assert(frames[w->self].empty());
    RD_DEBUG("Initial inserts by worker %i\n", w->self);
    init_strand(w, NULL); // sets t_worker
  }
  if (__cilkrts_get_batch_id(t_worker) != -1) return;
  int worker_id = t_worker->self;

  assert(!frames[worker_id].empty());

  FrameData_t* parent = frames[worker_id].head(); // helper frame parent
  parent->spawn_counter++;
  frames[worker_id].push();
  FrameData_t* f = frames[worker_id].head();
  f->enter_counter = 0;
  f->spawn_counter = 0;
  RD_DEBUG("helper_begin: enter_counter = 0 -- ");

  if (!parent->sync_english) { // first of spawn group
    assert(!parent->sync_hebrew);
    RD_DEBUG("first of spawn group\n");
    parent->sync_english =
      insert_or_relabel(t_worker, g_english, heavy_english, parent->current_english);
    parent->sync_hebrew =
      insert_or_relabel(t_worker, g_hebrew, heavy_hebrew, parent->current_hebrew);
  } else {
    RD_DEBUG("NOT first of spawn group\n");
  }

  f->current_english =
    insert_or_relabel(t_worker, g_english, heavy_english, parent->current_english);
  parent->cont_english =
    insert_or_relabel(t_worker, g_english, heavy_english, f->current_english);

  parent->cont_hebrew =
    insert_or_relabel(t_worker, g_hebrew, heavy_hebrew, parent->current_hebrew);
  f->current_hebrew =
    insert_or_relabel(t_worker, g_hebrew, heavy_hebrew, parent->cont_hebrew);
  
  f->cont_english = NULL;
  f->cont_hebrew = NULL;
  f->sync_english = NULL; 
  f->sync_hebrew = NULL;
  parent->current_english = NULL; 
  parent->current_hebrew = NULL;
}

extern "C" void cilk_enter_end (__cilkrts_stack_frame* sf, void* rsp)
{
  if (__cilkrts_get_batch_id(sf->worker) != -1) return;
  if (!frames[sf->worker->self].empty()) {
    FrameData_t* f = frames[sf->worker->self].head();
    f->enter_counter++;
    RD_DEBUG("cilk_enter_end: incrementing enter_counter to %zu\n", f->enter_counter);
  }
}

extern "C" void cilk_spawn_prepare (__cilkrts_stack_frame* sf) { }
extern "C" void cilk_spawn_or_continue (int in_continuation){ }
extern "C" void cilk_detach_begin (__cilkrts_stack_frame* parent) { }
extern "C" void cilk_detach_end (void) { }

extern "C" void cilk_sync_begin (__cilkrts_stack_frame* sf)
{ 
  if (__cilkrts_get_batch_id(sf->worker) != -1) return;
  if (frames[sf->worker->self].empty()) return;
  FrameData_t* f = frames[sf->worker->self].head();
  RD_DEBUG("cilk_sync_begin with g_enter_counter == %zu\n", f->enter_counter);

  /// @todo this isn't always correct!
  // Cilk function boo calls bar, which spawns quack, then immediately syncs
  //  if (f->enter_counter != 0) return;
  if (f->spawn_counter != 0) return;
  // real sync

  assert(f->current_english);
  assert(f->current_hebrew);
  assert(!f->cont_english); 
  assert(!f->cont_hebrew);

   if (f->sync_english) { // spawned
     assert(f->sync_hebrew);
       RD_DEBUG("function spawned (in cilk_sync_begin)\n");
       f->current_english = f->sync_english;
       f->current_hebrew = f->sync_hebrew;
       f->sync_english = NULL; 
       f->sync_hebrew = NULL;
  } else { // function didn't spawn
    assert(!f->sync_english); 
    assert(!f->sync_hebrew);
  }
}

extern "C" void cilk_sync_end (__cilkrts_stack_frame* sf){ }

extern "C" void cilk_leave_begin (__cilkrts_stack_frame *sf)
{
}

extern "C" void cilk_leave_end (void)
{ 
  if (__cilkrts_get_batch_id(t_worker) != -1) return;

  RD_DEBUG("cilk_leave_begin\n");

  /// @todo this shouldn't be necessary
  if (frames[t_worker->self].size() <= 1) return;

  FrameData_t* current = frames[t_worker->self].head();
  current->enter_counter--;
  RD_DEBUG("cilk_leave_begin, decrementing g_enter_counter to %zu\n", current->enter_counter);

  if (current->enter_counter == 0) { // leaving a spawn helper
    RD_DEBUG("cilk_leave_begin: popping and changing nodes.\n");

    frames[t_worker->self].pop();
    current = frames[t_worker->self].head();
    current->spawn_counter--;
    assert(!current->current_english);
    assert(!current->current_hebrew);
    assert(current->cont_english);
    assert(current->cont_hebrew);
    assert(current->sync_english);
    assert(current->sync_hebrew);
    current->current_english = current->cont_english;
    current->current_hebrew = current->cont_hebrew;
    current->cont_english = NULL;
    current->cont_hebrew = NULL;
  }

}
