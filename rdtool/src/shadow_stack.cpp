#include <iostream>

#include <internal/abi.h>
#include <cilk/batcher.h>
#include "shadow_stack.h"
#include "print_addr.h"
#include "shadow_mem.h"
#include "mem_access.h"

//enum frame_t { FRAME_USER = 0, FRAME_HELPER };
#define FRAME_HELPER_MASK 0x1
#define FRAME_FULL_MASK 0x2
#include "stack.h"
#include "om/om.h"

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
unsigned long g_relabel_size = 0;
#endif

#if STATS > 0
#define RD_STATS(...) __VA_ARGS__
#else
#define RD_STATS(...)
#endif

local_mut* g_worker_mutexes;

int g_tool_init = 0;
om* g_english;
om* g_hebrew;
__thread __cilkrts_worker* t_worker = NULL;
__thread int self = -1;
pthread_spinlock_t g_relabel_mutex;

typedef struct FrameData_s {
  om_node* current_english;
  om_node* current_hebrew;
  om_node* cont_english;
  om_node* cont_hebrew;
  om_node* sync_english;
  om_node* sync_hebrew;
  uint32_t flags;
} FrameData_t;

AtomicStack_t<FrameData_t>* frames;
om_node* base_english = NULL;
om_node* base_hebrew = NULL;

/// @todo A different design would be to have each OM itself contain a
/// list of heavy nodes to rebalance.
AtomicStack_t<tl_node*> heavy_english;
AtomicStack_t<tl_node*> heavy_hebrew;

// XXX Need to synchronize access to it when update
static ShadowMem<MemAccessList_t> shadow_mem;

pthread_spinlock_t g_insert_lock; /// @todo remove
om_node* insert_wrapper(om* ds, om_node* base)
{
  om_node* n;
  //  pthread_spin_lock(&g_insert_lock);
  if (base == NULL) n = om_insert_initial(ds);
  else n = om_insert(ds, base);
  if (base) {
    om_assert(base->list->above);
  }
  if (n) {
    om_assert(n->list->above);
  }
  //  pthread_spin_unlock(&g_insert_lock);
  return n;
}

#define HALF_BITS ((sizeof(label_t) * 8) / 2)

void init_strand(__cilkrts_worker* w, FrameData_t* init)
{
  self = w->self;
  t_worker = w;

  FrameData_t* f;
  if (init) {
    f = frames[w->self].head();
    assert(!(init->flags & FRAME_HELPER_MASK));
    assert(!init->current_english);
    assert(!init->current_hebrew);
    locked_assert(om_precedes(init->cont_english, init->sync_english));
    locked_assert(om_precedes(init->cont_hebrew, init->sync_hebrew));

    //    pthread_spin_lock(&g_worker_mutexes[self]);
    f->current_english = init->cont_english;
    f->current_hebrew = init->cont_hebrew;
    f->sync_english = init->sync_english;
    f->sync_hebrew = init->sync_hebrew;
    f->cont_english = NULL;
    f->cont_hebrew = NULL;
    f->flags = init->flags;
    //    pthread_spin_unlock(&g_worker_mutexes[self]);
  }  else {
    assert(frames[w->self].empty());
    frames[w->self].push();
    f = frames[w->self].head();
    f->flags = 0;
    f->current_english = base_english ? base_english : insert_wrapper(g_english, NULL);
    f->current_hebrew = base_hebrew ? base_hebrew : insert_wrapper(g_hebrew, NULL);
    // f->current_english = insert_wrapper(g_english, NULL);
    // f->current_hebrew = insert_wrapper(g_hebrew, NULL);
    //    RD_STATS(__sync_fetch_and_add(&g_num_inserts, 2));
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

/// @todo either find a way to pass this through batchify or store it
/// in the OM DS.
__thread AtomicStack_t<tl_node*>* saved_heavy_nodes;
int g_batch_owner_id = -1;

// We actually only need the DS.
void batch_relabel(void* ds, void* data, size_t size, void* results)
{
  RD_DEBUG("Begin relabel %zu.\n", g_num_relabels);
  om_assert(saved_heavy_nodes != NULL);

  // Save this because a different thread may return. @todo change
  // batcher so this doesn't happen
  AtomicStack_t<tl_node*>* heavy_nodes = saved_heavy_nodes;
  if (!heavy_nodes->empty())
    om_relabel((om*)ds, saved_heavy_nodes->at(0), saved_heavy_nodes->size());

  RD_STATS(g_num_relabels++);
  RD_STATS(g_relabel_size += heavy_nodes->size());

  heavy_nodes->reset();

}

void join_batch(om* ds, AtomicStack_t<tl_node*>& heavy_nodes)
{
  RD_DEBUG("Worker %i calling batchify.\n", self);
  saved_heavy_nodes = &heavy_nodes;
  cilk_batchify(batch_relabel, ds, 0, sizeof(int));
  saved_heavy_nodes = NULL;
  if (self == g_batch_owner_id) {
    g_batch_owner_id = -1;
    for (int i = 0; i < __cilkrts_get_nworkers(); ++i) {
      pthread_spin_unlock(&g_worker_mutexes[i].mut);
    }
    pthread_spin_unlock(&g_relabel_mutex);
  }
}

extern "C" int cilk_tool_om_try_lock_all(__cilkrts_worker* w)
{
  if (pthread_spin_trylock(&g_relabel_mutex) != 0) return 0;
  int p = __cilkrts_get_nworkers();
  for (int i = 0; i < p; ++i) {
    pthread_spin_lock(&g_worker_mutexes[i].mut);
  }
  cilk_set_next_batch_owner();
  om_assert(g_batch_owner_id == -1);
  g_batch_owner_id = w->self;
  return 1;
}

om_node* insert_or_relabel(__cilkrts_worker* w, om* ds,
                           AtomicStack_t<tl_node*>& heavy_nodes, om_node* base)
{
  pthread_spinlock_t* mut = &g_worker_mutexes[w->self].mut;
  while (pthread_spin_trylock(mut) != 0) {
    join_batch(ds, heavy_nodes);
  }

  om_node* n = insert_wrapper(ds, base);
  while (!n) {
    if (base->list->half_full == 0
	&& __sync_bool_compare_and_swap(&base->list->half_full, 0, 1)) {
      RD_DEBUG("Could not fit into list of size %zu, not half full.\n", base->list->size);
      heavy_nodes.push();
      *heavy_nodes.head() = om_get_tl(base);
      assert(base->list->above->level == MAX_LEVEL);
    }

    pthread_spin_unlock(mut);
    // if (cilk_tool_om_try_lock_all(w)) {
    //   if (heavy_nodes.empty()) assert(base->list->half_full == 0);
    //   else start_batch(ds, heavy_nodes);
    // } else {
    //   join_batch();
    // }
    join_batch(ds, heavy_nodes);
    
    while (pthread_spin_trylock(mut) != 0) {
      join_batch(ds, heavy_nodes);
    }
    n = insert_wrapper(ds, base);
  }
  assert(n->list == base->list);
  if (!n->list->half_full
      && count_set_bits(n->label) >= HALF_BITS) {
    if (__sync_bool_compare_and_swap(&base->list->half_full, 0, 1)) {
      RD_DEBUG("List marked as half full with %zu items.\n", n->list->size);
      heavy_nodes.push();
      *heavy_nodes.head() = om_get_tl(n);
      assert(n->list->above->level == MAX_LEVEL);
    }
  }
  pthread_spin_unlock(mut);
  RD_STATS(__sync_fetch_and_add(&g_num_inserts, 1));
  return n;
}

extern "C" void __om_init()
{
  g_english = om_new();
  g_hebrew = om_new();
  base_english = om_insert_initial(g_english);
  base_hebrew = om_insert_initial(g_hebrew);
  RD_STATS(__sync_fetch_and_add(&g_num_inserts, 2));
}


extern "C" void do_tool_init(void) 
{
  DBG_TRACE(DEBUG_CALLBACK, "cilk_tool_init called.\n");

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
  if (!base_english) {
    g_english = om_new();
    g_hebrew = om_new();
  }
  int p = __cilkrts_get_nworkers();
  frames = new AtomicStack_t<FrameData_t>[p];
  //  g_worker_mutexes = new pthread_spinlock_t[p];
  g_worker_mutexes = (local_mut*)memalign(64, sizeof(local_mut)*p);
  for (int i = 0; i < p; ++i) {
    pthread_spin_init(&g_worker_mutexes[i].mut, PTHREAD_PROCESS_PRIVATE);
  }
  pthread_spin_init(&g_relabel_mutex, PTHREAD_PROCESS_PRIVATE);
  pthread_spin_init(&g_insert_lock, PTHREAD_PROCESS_PRIVATE);

  // XXX: Can I assume this is called before everything?
  read_proc_maps();
  g_tool_init = 1;
}


int g_counter = 0;

extern "C" void do_tool_print(void)
{
  DBG_TRACE(DEBUG_CALLBACK, "cilk_tool_print called.\n");

#if STATS > 1
  for (int i = 0; i < NUM_INTERVAL_TYPES; ++i) {
    assert(g_timing_event_starts[i].tv_sec == EMPTY_TIME_POINT.tv_sec);
    assert(g_timing_event_starts[i].tv_nsec == EMPTY_TIME_POINT.tv_nsec);
    std::cout << "\t\t" << g_timing_events[i];
  }
  std::cout << std::endl;
#endif
  std::cout << "counter: " << g_counter << std::endl;
#if STATS > 0  
  std::cout << "Num relabels: " << g_num_relabels << std::endl;
  std::cout << "Num inserts: " << g_num_inserts << std::endl;
  std::cout << "Avg size per relabel: " << (double)g_relabel_size / (double)g_num_relabels << std::endl;
#endif
}

extern "C" void do_tool_destroy(void) 
{
  DBG_TRACE(DEBUG_CALLBACK, "cilk_tool_destroy called.\n");

  do_tool_print();
  om_free(g_english);
  om_free(g_hebrew);
  delete[] frames;

  for (int i = 0; i < __cilkrts_get_nworkers(); ++i) {
    pthread_spin_destroy(&g_worker_mutexes[i].mut);
  }
  pthread_spin_destroy(&g_relabel_mutex);
  pthread_spin_destroy(&g_insert_lock);

  //  delete[] g_worker_mutexes;
  free(g_worker_mutexes);
  //  delete_proc_maps(); /// @todo shakespeare has problems compiling print_addr.cpp
}

extern "C" om_node* get_current_english()
{
  __cilkrts_worker* w = __cilkrts_get_tls_worker();
  if (!frames) return NULL;
  return frames[w->self].head()->current_english;
}

extern "C" om_node* get_current_hebrew()
{
  __cilkrts_worker* w = __cilkrts_get_tls_worker();
  if (!frames) return NULL;
  return frames[w->self].head()->current_hebrew;
}


extern "C" void do_steal_success(__cilkrts_worker* w, __cilkrts_worker* victim,
                                 __cilkrts_stack_frame* sf)
{
  DBG_TRACE(DEBUG_CALLBACK, "Worker %i stole from %i.\n", w->self, victim->self);

  // Lock b/c we might copy om_nodes that are being relabeled...
  // Actually, these shouldn't be necessary, because the bottom level
  // nodes never actually change, just the pointers to the above
  // tl_nodes
  /// @todo remove lock
  //  pthread_spin_lock(&g_worker_mutexes[w->self]);
  frames[w->self].reset();
  FrameData_t* loot = frames[victim->self].steal_top(frames[w->self]);
  //  pthread_spin_unlock(&g_worker_mutexes[w->self]);
  om_assert(!(loot->flags & FRAME_HELPER_MASK));
  init_strand(w, loot);
}

extern "C" void cilk_return_to_first_frame(__cilkrts_worker* w, __cilkrts_worker* team,
					   __cilkrts_stack_frame* sf)
{
  DBG_TRACE(DEBUG_CALLBACK, "Transfering shadow stack from %d to original thread %d.\n", self, team->self);
  if (self != team->self) frames[self].transfer(frames[team->self]);
}

extern "C" void do_enter_begin()
{
  if (self == -1) return;
  frames[self].push();
  FrameData_t* parent = frames[self].ancestor(1);
  FrameData_t* f = frames[self].head();
  f->flags = 0;
  f->cont_english = f->cont_hebrew = NULL;
  f->sync_english = f->sync_hebrew = NULL;
  f->current_english = parent->current_english;
  f->current_hebrew = parent->current_hebrew;
}

  //  DBG_TRACE(DEBUG_CALLBACK, "do_detach_begin, parent sf %p.\n", sf->call_parent);
extern "C" void do_enter_helper_begin(__cilkrts_stack_frame* sf, void* this_fn, void* rip) { }

extern "C" void do_detach_begin(__cilkrts_stack_frame* parent_sf)
//extern "C" void do_detach_end()
{
  // DBG_TRACE(DEBUG_CALLBACK, "do_detach_begin, parent %p and worker %d.\n",
  //           parent_sf, self);
  DBG_TRACE(DEBUG_CALLBACK, "do_detach_end, worker %d.\n", self);


  __cilkrts_worker* w = __cilkrts_get_tls_worker();

  // can't be empty now that we init strand in do_enter_end
  om_assert(!frames[w->self].empty());

  frames[w->self].push_helper();
  FrameData_t* parent = frames[w->self].ancestor(1);
  FrameData_t* f = frames[w->self].head();
  assert(parent + 1 == f);

  assert(!(parent->flags & FRAME_HELPER_MASK));
  f->flags = FRAME_HELPER_MASK;

  if (!parent->sync_english) { // first of spawn group
    om_assert(!parent->sync_hebrew);
    RD_DEBUG("first of spawn group\n");

    parent->sync_english =
      insert_or_relabel(w, g_english, heavy_english, parent->current_english);
    parent->sync_hebrew =
      insert_or_relabel(w, g_hebrew, heavy_hebrew, parent->current_hebrew);

    // pthread_spin_lock(&g_worker_mutexes[self]);
    // assert(om_precedes(parent->current_english, parent->sync_english));
    // assert(om_precedes(parent->current_hebrew, parent->sync_hebrew));
    // pthread_spin_unlock(&g_worker_mutexes[self]);

    DBG_TRACE(DEBUG_CALLBACK, 
        "do_detach_begin, setting parent sync_eng %p and sync heb %p.\n",
        parent->sync_english, parent->sync_hebrew);
  } else {
    DBG_TRACE(DEBUG_CALLBACK, 
              "do_detach_begin, NOT first spawn, parent sync eng %p.\n", 
              parent->sync_english);
    RD_DEBUG("NOT first of spawn group\n");
  }

  f->current_english =
    insert_or_relabel(w, g_english, heavy_english, parent->current_english);
  //  locked_assert(om_precedes(parent->current_english, f->current_english));
  parent->cont_english =
    insert_or_relabel(w, g_english, heavy_english, f->current_english);
  //  locked_assert(om_precedes(f->current_english, parent->cont_english));

  parent->cont_hebrew =
    insert_or_relabel(w, g_hebrew, heavy_hebrew, parent->current_hebrew);
  //  locked_assert(om_precedes(parent->current_hebrew, parent->cont_hebrew));
  f->current_hebrew =
    insert_or_relabel(w, g_hebrew, heavy_hebrew, parent->cont_hebrew);
  //  locked_assert(om_precedes(parent->cont_hebrew, f->current_hebrew));
  
  DBG_TRACE(DEBUG_CALLBACK, 
            "do_detach_begin, f curr eng: %p and parent cont eng: %p.\n", 
            f->current_english, parent->cont_english);
  DBG_TRACE(DEBUG_CALLBACK, 
            "do_detach_begin, f curr heb: %p and parent cont heb: %p.\n", 
            f->current_hebrew, parent->cont_hebrew);

  f->cont_english = NULL;
  f->cont_hebrew = NULL;
  f->sync_english = NULL; 
  f->sync_hebrew = NULL;
  parent->current_english = NULL; 
  parent->current_hebrew = NULL;
}

/* return 1 if we are entering top-level user frame and 0 otherwise */
extern "C" int do_enter_end (__cilkrts_stack_frame* sf, void* rsp)
{
  DBG_TRACE(DEBUG_CALLBACK, "do_enter_end, sf %p and worker %d.\n", sf, self);

  __cilkrts_worker* w = sf->worker;
  if (__cilkrts_get_batch_id(w) != -1) return 0;

  if (frames[w->self].empty()) {
    self = w->self;
    init_strand(w, NULL); // enter_counter already set to 1 in init_strand
    return 1;
  } else {
    FrameData_t* f = frames[sf->worker->self].head();
    return 0;
  }
}

extern "C" void do_sync_begin (__cilkrts_stack_frame* sf)
{
  DBG_TRACE(DEBUG_CALLBACK, "do_sync_begin, sf %p and worker %d.\n", sf, self);

  om_assert(self != -1);
  om_assert(!frames[self].empty());

  if (__cilkrts_get_batch_id(sf->worker) != -1) return;

  FrameData_t* f = frames[self].head();
  DBG_TRACE(DEBUG_CALLBACK, "do_sync_begin, shadow frame %p and worker %d.\n", f, self);

  if (f->flags & FRAME_HELPER_MASK) { // this is a stolen frame, and this worker will be returning to the runtime shortly
    return; /// @todo is this correct? this only seems to happen on the initial frame, when it is stolen
  }

  //  assert(!(f->flags & FRAME_HELPER_MASK));
  om_assert(f->current_english); 
  om_assert(f->current_hebrew);
  om_assert(!f->cont_english); 
  om_assert(!f->cont_hebrew);

  if (f->sync_english) { // spawned
    om_assert(f->sync_hebrew);
    DBG_TRACE(DEBUG_CALLBACK, "function spawned (in cilk_sync_begin) (worker %d)\n", self);

    locked_assert(om_precedes(f->current_english, f->sync_english));
    locked_assert(om_precedes(f->current_hebrew, f->sync_hebrew));
    f->current_english = f->sync_english;
    f->current_hebrew = f->sync_hebrew;
    DBG_TRACE(DEBUG_CALLBACK, 
              "do_sync_begin, setting eng to %p.\n", f->current_english);
    DBG_TRACE(DEBUG_CALLBACK, 
              "do_sync_begin, setting heb to %p.\n", f->current_hebrew);
    f->sync_english = NULL; 
    f->sync_hebrew = NULL;
  } else { // function didn't spawn, or this is a function-ending sync
    om_assert(!f->sync_english); 
    om_assert(!f->sync_hebrew);
  }
}

// Noticed that the frame was stolen.
extern "C" void do_leave_stolen(__cilkrts_stack_frame* sf)
{
  DBG_TRACE(DEBUG_CALLBACK, "do_leave_stolen, sf %p and worker %d.\n", sf, self);

  // We *should* still have a helper frame on the shadow stack, since
  // cilk_leave_end will not be called
  FrameData_t* child = frames[self].head();
  FrameData_t* parent = frames[self].ancestor(1);

  om_assert(child->flags & FRAME_HELPER_MASK);
  om_assert(!(parent->flags & FRAME_HELPER_MASK));
  om_assert(parent->current_english == NULL);
  om_assert(parent->current_hebrew == NULL);
  om_assert(parent->cont_english);
  om_assert(parent->cont_hebrew);
  om_assert(parent->sync_english);
  om_assert(parent->sync_hebrew);

  parent->current_english = parent->sync_english;
  parent->current_hebrew = parent->sync_hebrew;
  parent->cont_english = NULL;
  parent->cont_hebrew = NULL;
  parent->sync_english = NULL;
  parent->sync_hebrew = NULL;

  frames[self].pop();
}

/* return 1 if we are leaving last frame and 0 otherwise */
extern "C" int do_leave_begin (__cilkrts_stack_frame *sf)
{
  return frames[sf->worker->self].empty();
}


extern "C" int do_leave_end()
{
  //  DBG_TRACE(DEBUG_CALLBACK, "do_leave_end, sf %p and worker %d.\n", sf, self);
  DBG_TRACE(DEBUG_CALLBACK, "do_leave_end, worker %d.\n", self);
  assert(t_worker);
  if (__cilkrts_get_batch_id(t_worker) != -1) return 0;

  om_assert(self != -1);
  om_assert(!frames[self].empty());

  FrameData_t* child = frames[self].head();
  if (frames[self].size() > 1) {
    DBG_TRACE(DEBUG_CALLBACK, "cilk_leave_end(%d): popping and changing nodes.\n", self);
    FrameData_t* parent = frames[self].ancestor(1);
    if (!(child->flags & FRAME_HELPER_MASK)) { // parent called current
      DBG_TRACE(DEBUG_CALLBACK, "cilk_leave_end(%d): returning from call.\n", self);

      // pthread_spin_lock(&g_worker_mutexes[self]);
      // assert(parent->current_english == child->current_english
	    //  || om_precedes(parent->current_english, child->current_english));
      // assert(parent->current_hebrew == child->current_hebrew
	    //  || om_precedes(parent->current_hebrew, child->current_hebrew));
      // pthread_spin_unlock(&g_worker_mutexes[self]);

      om_assert(!parent->cont_english);
      om_assert(!parent->cont_hebrew);
      parent->current_english = child->current_english;
      parent->current_hebrew = child->current_hebrew;
    } else { // parent spawned current
      DBG_TRACE(DEBUG_CALLBACK, "cilk_leave_end(%d): parent spawned child.\n", self);
      om_assert(!parent->current_english); om_assert(!parent->current_hebrew);
      om_assert(parent->cont_english); om_assert(parent->cont_hebrew);
      om_assert(parent->sync_english); om_assert(parent->sync_hebrew);
      parent->current_english = parent->cont_english;
      parent->current_hebrew = parent->cont_hebrew;
      parent->cont_english = parent->cont_hebrew = NULL;
    }
    frames[self].pop();
  }

  // we are leaving the last frame if the shadow stack is empty
  return frames[self].empty();
}

// called by record_memory_read/write, with the access broken down 
// into 8-byte aligned memory accesses
void 
record_mem_helper(bool is_read, uint64_t inst_addr, uint64_t addr,
                  uint32_t mem_size)
{
  om_assert(self != -1);
  FrameData_t *f = frames[self].head();
  MemAccessList_t *val = shadow_mem.find( ADDR_TO_KEY(addr) );
  MemAccess_t *acc = NULL;
  MemAccessList_t *mem_list = NULL;

  if( val == NULL ) {
    g_counter++;
    // not in shadow memory; create a new MemAccessList_t and insert
    //    if (acc == NULL) {
      acc = new MemAccess_t(f->current_english, f->current_hebrew, inst_addr);
      mem_list = new MemAccessList_t(addr, is_read, acc, mem_size);
      //    }
    val = shadow_mem.insert(ADDR_TO_KEY(addr), mem_list);
    // insert failed; someone got to the slot first;
    // delete new mem_list and fall through to check race with it 
    if( val != mem_list ) { delete mem_list; }
    else { return; } // else we are done
  }
  // check for race and possibly update the existing MemAccessList_t 
  om_assert(val != NULL);
  val->check_races_and_update(is_read, inst_addr, addr, mem_size, 
                              f->current_english, f->current_hebrew);
}

// XXX: We can only read 1,2,4,8,16 bytes; optimize later
extern "C" void do_read(uint64_t inst_addr, uint64_t addr, size_t mem_size)
{
  DBG_TRACE(DEBUG_MEMORY, "record read of %lu bytes at addr %p and rip %p.\n", 
            mem_size, addr, inst_addr);

  // handle the prefix
  uint64_t next_addr = ALIGN_BY_NEXT_MAX_GRAIN_SIZE(addr); 
  size_t prefix_size = next_addr - addr;
  om_assert(prefix_size >= 0 && prefix_size < MAX_GRAIN_SIZE);

  if(prefix_size >= mem_size) { // access falls within a max grain sized block
    record_mem_helper(true, inst_addr, addr, mem_size);
  } else { 
    om_assert( prefix_size <= mem_size );
    if(prefix_size) { // do the prefix first
      record_mem_helper(true, inst_addr, addr, prefix_size);
      mem_size -= prefix_size;
    }
    addr = next_addr;
    // then do the rest of the max-grain size aligned blocks
    uint32_t i;
    for(i = 0; (i+MAX_GRAIN_SIZE) < mem_size; i += MAX_GRAIN_SIZE) {
      record_mem_helper(true, inst_addr, addr + i, MAX_GRAIN_SIZE);
    }
    // trailing bytes
    record_mem_helper(true, inst_addr, addr+i, mem_size-i);
  }
}

// XXX: We can only read 1,2,4,8,16 bytes; optimize later
extern "C" void do_write(uint64_t inst_addr, uint64_t addr, size_t mem_size)
{
  DBG_TRACE(DEBUG_MEMORY, "record write of %lu bytes at addr %p and rip %p.\n", 
            mem_size, addr, inst_addr);

  // handle the prefix
  uint64_t next_addr = ALIGN_BY_NEXT_MAX_GRAIN_SIZE(addr); 
  size_t prefix_size = next_addr - addr;
  om_assert(prefix_size >= 0 && prefix_size < MAX_GRAIN_SIZE);

  if(prefix_size >= mem_size) { // access falls within a max grain sized block
    record_mem_helper(false, inst_addr, addr, mem_size);
  } else {
    om_assert( prefix_size <= mem_size );
    if(prefix_size) { // do the prefix first
      record_mem_helper(false, inst_addr, addr, prefix_size);
      mem_size -= prefix_size;
    }
    addr = next_addr;
    // then do the rest of the max-grain size aligned blocks
    uint32_t i=0;
    for(i=0; (i+MAX_GRAIN_SIZE) < mem_size; i += MAX_GRAIN_SIZE) {
      record_mem_helper(false, inst_addr, addr + i, MAX_GRAIN_SIZE);
    }
    // trailing bytes
    record_mem_helper(false, inst_addr, addr+i, mem_size-i);
  }
}

// clear the memory block at [start-end) (end is exclusive).
extern "C" void clear_shadow_memory(size_t start, size_t end) {

  DBG_TRACE(DEBUG_MEMORY, "Clear shadow memory %p--%p (%u).\n", 
            start, end, end-start);
  om_assert(ALIGN_BY_NEXT_MAX_GRAIN_SIZE(end) == end); 
  om_assert(start < end);
  //  om_assert(end-start < 4096);

  while(start != end) {
    // DBG_TRACE(DEBUG_MEMORY, "Erasing mem %p.\n", (void *)start);
    shadow_mem.erase( ADDR_TO_KEY(start) );
    start += MAX_GRAIN_SIZE;
  }
}
