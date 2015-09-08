#include <iostream>
#include <pthread.h> /// @todo clang doesn't like gcc 4.4.7 mutex header...

#include <internal/abi.h>
#include <cilk/batcher.h>
#include "print_addr.h"
#include "shadow_mem.h"
#include "mem_access.h"
#include "stack.h"
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
__thread int self = -1;
pthread_spinlock_t g_relabel_mutex;
pthread_spinlock_t* g_worker_mutexes;

enum frame_t { FRAME_USER, FRAME_HELPER };

typedef struct FrameData_s {
  om_node* current_english;
  om_node* current_hebrew;
  om_node* cont_english;
  om_node* cont_hebrew;
  om_node* sync_english;
  om_node* sync_hebrew;
  // we only create frames for the first Cilk function and whenever
  // we spawn, so multiple Cilk functions can be represented by this frame
  // (i.e., a Cilk function calls another Cilk function)
  // this counter is counting the # of Cilk functions this frame represents
  //  int enter_counter; 
  // store the value of enter_counter for the Cilk function that spawned to
  // cause the sync node to be set; we should only switch the current node
  // to the sync node when precisely that Cilk function is executing the sync
  //  int enter_counter_for_sync;
  enum frame_t type;
} FrameData_t;

AtomicStack_t<FrameData_t>* frames;

/// @todo A different design would be to have each OM itself contain a
/// list of heavy nodes to rebalance.
AtomicStack_t<tl_node*> heavy_english;
AtomicStack_t<tl_node*> heavy_hebrew;

// XXX Need to synchronize access to it when update
static ShadowMem<MemAccessList_t> shadow_mem;
/*
 It turns out that we don't need to know where the stack is
uint64_t stack_low_addr = 0;
uint64_t stack_high_addr = 0;

__attribute__((always_inline)) 
static bool is_on_stack(uint64_t addr) {
  om_assert(stack_high_addr != stack_low_addr);                            
  return (addr <= stack_high_addr && addr >= stack_low_addr);                   
} 
*/


#define HALF_BITS ((sizeof(label_t) * 8) / 2)

void init_strand(__cilkrts_worker* w, FrameData_t* init)
{
  self = w->self;
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
    //    f->enter_counter = init->enter_counter;
    //    f->enter_counter_for_sync = init->enter_counter_for_sync;
    f->type = init->type;
  }  else {
    // special case; this one starts with 1, since this is called from enter_end
    //    f->enter_counter = 1;
    //    f->enter_counter_for_sync = -1; // -1 for unset value
    f->type = FRAME_USER;
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
  DBG_TRACE(DEBUG_CALLBACK, "cilk_tool_print called.\n");

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

  // XXX: Can I assume this is called before everything?
  //  read_proc_maps(); /// @todo shakespeare has problems compiling print_addr.cpp
  g_tool_init = 1;
}

extern "C" void cilk_tool_print(void)
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
#if STATS > 0  
  std::cout << "Num relabels: " << g_num_relabels << std::endl;
  std::cout << "Num inserts: " << g_num_inserts << std::endl;
#endif
}

extern "C" void cilk_tool_destroy(void) 
{
  DBG_TRACE(DEBUG_CALLBACK, "cilk_tool_destroy called.\n");

  cilk_tool_print();
  om_free(g_english);
  om_free(g_hebrew);
  delete[] frames;

  for (int i = 0; i < __cilkrts_get_nworkers(); ++i) {
    pthread_spin_destroy(&g_worker_mutexes[i]);
  }
  pthread_spin_destroy(&g_relabel_mutex);

  delete[] g_worker_mutexes;
  //  delete_proc_maps(); /// @todo shakespeare has problems compiling print_addr.cpp
}

extern "C" void cilk_steal_success(__cilkrts_worker* w, __cilkrts_worker* victim,
				  __cilkrts_stack_frame* sf)
{
  RD_DEBUG("Worker %i stole from %i\n", w->self, victim->self);
  frames[w->self].reset();
  FrameData_t* loot = (FrameData_t*)malloc(sizeof(FrameData_t));
  frames[victim->self].steal_top(loot);
  if (loot->type == FRAME_HELPER) frames[victim->self].steal_top(loot);
  assert(loot->type == FRAME_USER);
  init_strand(w, loot);
  free(loot);
}

extern "C" void do_enter_begin()
{
  if (self == -1) return;
  frames[self].push();
  FrameData_t* parent = frames[self].ancestor(1);
  FrameData_t* f = frames[self].head();
  f->type = FRAME_USER;
  f->cont_english = f->cont_hebrew = NULL;
  f->sync_english = f->sync_hebrew = NULL;
  f->current_english = parent->current_english;
  f->current_hebrew = parent->current_hebrew;
}

// XXX Some of these should be done in detach --- args are evaluated between
// enter_helper_begin and detach
extern "C" void do_enter_helper_begin(__cilkrts_stack_frame* sf, void* this_fn, void* rip)
{
  DBG_TRACE(DEBUG_CALLBACK, "do_enter_helper_begin, sf %p and worker %d.\n", sf, self);
  DBG_TRACE(DEBUG_CALLBACK, "do_enter_helper_begin, parent sf %p.\n", sf->call_parent);

  __cilkrts_worker* w = __cilkrts_get_tls_worker();
  if (__cilkrts_get_batch_id(w) != -1) return;

  // can't be empty now that we init strand in do_enter_end
  om_assert(!frames[w->self].empty());

  // Can't do this, since the stack may resize
  //  FrameData_t* parent = frames[w->self].head(); // helper frame parent

  frames[w->self].push();
  FrameData_t* parent = frames[w->self].ancestor(1);
  FrameData_t* f = frames[w->self].head();
  assert(parent + 1 == f);
  //  f->enter_counter = 0; // eventually gets incremented in enter_end
  //  f->enter_counter_for_sync = -1; 
  assert(parent->type == FRAME_USER);
  f->type = FRAME_HELPER;
  RD_DEBUG("helper_begin: enter_counter = 0 -- ");

  if (!parent->sync_english) { // first of spawn group
    om_assert(!parent->sync_hebrew);
    //    om_assert(parent->enter_counter_for_sync == -1);
    RD_DEBUG("first of spawn group\n");

    //    parent->enter_counter_for_sync = parent->enter_counter;
    parent->sync_english =
      insert_or_relabel(w, g_english, heavy_english, parent->current_english);
    parent->sync_hebrew =
      insert_or_relabel(w, g_hebrew, heavy_hebrew, parent->current_hebrew);

    // DBG_TRACE(DEBUG_CALLBACK, 
    //     "do_enter_helper_begin, parent enter counter for sync %d.\n",
    //     parent->enter_counter_for_sync);

    DBG_TRACE(DEBUG_CALLBACK, 
        "do_enter_helper_begin, setting parent sync_eng %p and sync heb %p.\n",
        parent->sync_english, parent->sync_hebrew);
  } else {
    DBG_TRACE(DEBUG_CALLBACK, 
              "do_enter_helper_begin, NOT first spawn, parent sync eng %p.\n", 
              parent->sync_english);
    RD_DEBUG("NOT first of spawn group\n");
  }

  f->current_english =
    insert_or_relabel(w, g_english, heavy_english, parent->current_english);
  parent->cont_english =
    insert_or_relabel(w, g_english, heavy_english, f->current_english);

  parent->cont_hebrew =
    insert_or_relabel(w, g_hebrew, heavy_hebrew, parent->current_hebrew);
  f->current_hebrew =
    insert_or_relabel(w, g_hebrew, heavy_hebrew, parent->cont_hebrew);
  
  DBG_TRACE(DEBUG_CALLBACK, 
            "do_enter_helper_begin, f curr eng: %p and parent cont eng: %p.\n", 
            f->current_english, parent->cont_english);
  DBG_TRACE(DEBUG_CALLBACK, 
            "do_enter_helper_begin, f curr heb: %p and parent cont heb: %p.\n", 
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
    //    f->enter_counter++;
    //    RD_DEBUG("cilk_enter_end: incrementing enter_counter to %zu\n", f->enter_counter);
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
  //  RD_DEBUG("cilk_sync_begin with g_enter_counter == %zu\n", f->enter_counter);
  // since each frame can account for multiple Cilk functions, we are only
  // switching the current to the sync node when we reach the corresponding
  // sync statement 
  //  om_assert(f->enter_counter >= f->enter_counter_for_sync);
  //  if (f->enter_counter > f->enter_counter_for_sync) return;

  om_assert(f->current_english); 
  om_assert(f->current_hebrew);
  om_assert(!f->cont_english); 
  om_assert(!f->cont_hebrew);

  if (f->sync_english) { // spawned
    om_assert(f->sync_hebrew);
    RD_DEBUG("function spawned (in cilk_sync_begin)\n");
    f->current_english = f->sync_english;
    f->current_hebrew = f->sync_hebrew;
    DBG_TRACE(DEBUG_CALLBACK, 
              "do_sync_begin, setting eng to %p.\n", f->current_english);
    DBG_TRACE(DEBUG_CALLBACK, 
              "do_sync_begin, setting heb to %p.\n", f->current_hebrew);
    f->sync_english = NULL; 
    f->sync_hebrew = NULL;
    //    f->enter_counter_for_sync = -1;
  } else { // function didn't spawn
    om_assert(!f->sync_english); 
    om_assert(!f->sync_hebrew);
    //    om_assert(f->enter_counter_for_sync == -1);
  }
}

/* return 1 if we are leaving last frame and 0 otherwise */
extern "C" int do_leave_begin (__cilkrts_stack_frame *sf)
{
  DBG_TRACE(DEBUG_CALLBACK, "do_leave_begin, sf %p and worker %d.\n", sf, self);
  if (__cilkrts_get_batch_id(sf->worker) != -1) return 0;

  RD_DEBUG("cilk_leave_begin\n");
  om_assert(self != -1);
  om_assert(!frames[self].empty());

  FrameData_t* child = frames[self].head();
  //  om_assert(current->enter_counter > current->enter_counter_for_sync);

  //  current->enter_counter--;
  //  RD_DEBUG("cilk_leave_begin, decrementing g_enter_counter to %zu\n", current->enter_counter);

  //  if (current->enter_counter == 0) {
  if (frames[self].size() > 1) {
    RD_DEBUG("cilk_leave_begin: popping and changing nodes.\n");
    FrameData_t* parent = frames[self].ancestor(1);
    if (child->type == FRAME_USER) { // parent called current
      om_assert(parent->current_english == child->current_english
		|| om_precedes(parent->current_english, child->current_english));
      om_assert(parent->current_hebrew == child->current_hebrew
		|| om_precedes(parent->current_hebrew, child->current_hebrew));
      om_assert(!parent->cont_english);
      om_assert(!parent->cont_hebrew);
      parent->current_english = child->current_english;
      parent->current_hebrew = child->current_hebrew;
    } else { // parent spawned current
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
static void 
record_mem_helper(bool is_read, uint64_t inst_addr, uint64_t addr,
                  uint32_t mem_size) {

  om_assert(self != -1);
  FrameData_t *f = frames[self].head();
  MemAccessList_t *val = shadow_mem.find( ADDR_TO_KEY(addr) );

  if( val == NULL ) {
    // not in shadow memory; create a new MemAccessList_t and insert
    MemAccess_t *acc = 
        new MemAccess_t(f->current_english, f->current_hebrew, inst_addr);
    MemAccessList_t *mem_list = 
        new MemAccessList_t(addr, is_read, acc, mem_size);
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
extern "C" void do_read(uint64_t inst_addr, uint64_t addr, size_t mem_size) {

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
extern "C" void do_write(uint64_t inst_addr, uint64_t addr, size_t mem_size) {

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

  while(start != end) {
    // DBG_TRACE(DEBUG_MEMORY, "Erasing mem %p.\n", (void *)start);
    shadow_mem.erase( ADDR_TO_KEY(start) );
    start += MAX_GRAIN_SIZE;
  }
}

// extern "C" void cilk_enter_helper_begin(__cilkrts_stack_frame* sf, void* this_fn, void* rip){}
// extern "C" void cilk_enter_end (__cilkrts_stack_frame* sf, void* rsp) {}
// extern "C" void cilk_spawn_prepare (__cilkrts_stack_frame* sf) { }
// extern "C" void cilk_spawn_or_continue (int in_continuation){ }
// extern "C" void cilk_detach_begin (__cilkrts_stack_frame* parent) { }
// extern "C" void cilk_detach_end (void) { }
// extern "C" void cilk_sync_begin (__cilkrts_stack_frame* sf) { }
// extern "C" void cilk_sync_end (__cilkrts_stack_frame* sf){ }
// extern "C" void cilk_leave_begin (__cilkrts_stack_frame *sf){ }
// extern "C" void cilk_leave_end (void) { }
// extern "C" void cilk_tool_c_function_enter(void* this_fn, void* rip){ }
// extern "C" void cilk_tool_c_function_leave(void* rip) { }
// extern "C" void cilk_enter_begin(__cilkrts_stack_frame* sf, void* this_fn, void* rip){ }

