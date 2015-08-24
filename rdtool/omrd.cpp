#include <iostream>

#include <internal/abi.h>
#include <cilk/batcher.h>
//#include "../om/om.h"
#include "../om/om.c"
#include "stack.h"

int g_tool_init = 0;
om* g_english;
om* g_hebrew;

typedef enum { USER, HELPER } frame_t;

typedef struct FrameData_s {
  om_node* current_english;
  om_node* current_hebrew;
  om_node* cont_english;
  om_node* cont_hebrew;
  om_node* sync_english;
  om_node* sync_hebrew;
  frame_t type;
} FrameData_t;

Stack_t<FrameData_t>* frames;

/// @todo A different design would be to have each OM itself contain a
/// list of heavy nodes to rebalance.
Stack_t<tl_node*> heavy_english;
Stack_t<tl_node*> heavy_hebrew;

#define HALF_BITS ((sizeof(label_t) * 8) / 2)

void init_strand(int worker_id)
{
  //  std::cout << "initial push\n";
  assert(frames[worker_id].size() == 1);
  FrameData_t* f = frames[worker_id].head();
  f->type = USER;
  f->current_english = om_insert_initial(g_english);
  f->current_hebrew = om_insert_initial(g_hebrew);
  f->cont_english = NULL;
  f->cont_hebrew = NULL;
  f->sync_english = NULL;
  f->sync_hebrew = NULL;
}

int get_all_locks()
{
  return 1;
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

om_node* insert_or_relabel(__cilkrts_worker* w, om* ds,
                           Stack_t<tl_node*>& heavy_nodes, om_node* base)
{
  // Get worker om lock
  om_node* n = om_insert(ds, base);
  while (!n) {
    if (!base->list->half_full) {
      *heavy_nodes.head() = om_get_tl(base);
      heavy_nodes.push();
    }

    om_relabel(ds, heavy_nodes.at(0), heavy_nodes.size() - 1);
    heavy_nodes.reset();

    n = om_insert(ds, base);
  }
  if (!n->list->half_full
      && count_set_bits(n->label) >= HALF_BITS) {
    n->list->half_full = 1;
    *heavy_nodes.head() = om_get_tl(n);
    heavy_nodes.push();
  }
  return n;
}

extern "C" void cilk_tool_init(void) 
{ 
  g_tool_init = 1;
  g_english = om_new();
  g_hebrew = om_new();
  frames = new Stack_t<FrameData_t>[__cilkrts_get_nworkers()];

  init_strand(__cilkrts_get_worker_number());
  /// @todo allocate worker locks
}

// Currently, I don't think this is ever called. @todo
extern "C" void cilk_tool_destroy(void) 
{ 
  om_free(g_english);
  om_free(g_hebrew);

  delete[] frames;
}

extern "C" void cilk_tool_print(void) { }

extern "C" void cilk_tool_c_function_enter(void* this_fn, void* rip)
{ 
  int id = __cilkrts_get_worker_number();
  if (frames[id].size() > 1) assert(frames[id].head()->type == USER);
}
extern "C" void cilk_tool_c_function_leave(void* rip) { }

extern "C" void cilk_enter_begin(__cilkrts_stack_frame* sf, void* this_fn, void* rip)
{
  /// Need to call it like this because sf->worker may not be
  /// correctly initialized.
  __cilkrts_worker* w = __cilkrts_get_tls_worker();
  if (w && __cilkrts_get_batch_id(w) != -1) return;

  if (!g_tool_init) cilk_tool_init();
  else if (w) assert(frames[w->self].head()->type == USER);
}

extern "C" void cilk_enter_helper_begin(__cilkrts_stack_frame* sf, void* this_fn, void* rip)
{ 
  __cilkrts_worker* w = __cilkrts_get_tls_worker();
  if (__cilkrts_get_batch_id(w) != -1) return;
  assert(frames[w->self].size() > 0); // want non-empty, but stack returns
                             // _head + 1

  FrameData_t* parent = frames[w->self].head(); // helper frame parent
  assert(parent->type == USER);
  frames[w->self].push(); frames[w->self].head()->type = HELPER;

  frames[w->self].push();
  FrameData_t* f = frames[w->self].head();
  f->type = USER;

  if (!parent->sync_english) { // first of spawn group
    assert(!parent->sync_hebrew);
    parent->sync_english =
      insert_or_relabel(w, g_english, heavy_english, parent->current_english);
    parent->sync_hebrew =
      insert_or_relabel(w, g_hebrew, heavy_hebrew, parent->current_hebrew);
  }

  f->current_english =
    insert_or_relabel(w, g_english, heavy_english, parent->current_english);
  parent->cont_english =
    insert_or_relabel(w, g_english, heavy_english, f->current_english);

  parent->cont_hebrew =
    insert_or_relabel(w, g_hebrew, heavy_hebrew, parent->current_hebrew);
  f->current_hebrew =
    insert_or_relabel(w, g_hebrew, heavy_hebrew, parent->cont_hebrew);

  f->cont_english = NULL; f->cont_hebrew = NULL;
  f->sync_english = NULL; f->sync_hebrew = NULL;
  parent->current_english = NULL; parent->current_hebrew = NULL;
}

extern "C" void cilk_enter_end (__cilkrts_stack_frame* sf, void* rsp) { }

extern "C" void cilk_spawn_prepare (__cilkrts_stack_frame* sf) { }

extern "C" void cilk_spawn_or_continue (int in_continuation)
{ 
  assert(!in_continuation);
}

extern "C" void cilk_detach_begin (__cilkrts_stack_frame* parent) { }

extern "C" void cilk_detach_end (void) { }

extern "C" void cilk_sync_begin (__cilkrts_stack_frame* sf) { }

extern "C" void cilk_sync_end (__cilkrts_stack_frame* sf)
{ 
  if (__cilkrts_get_batch_id(sf->worker) != -1) return;
  FrameData_t* f = frames[sf->worker->self].head();
  assert(f->current_english); assert(f->current_hebrew);
  assert(!f->cont_english); assert(!f->cont_hebrew);

  if (f->sync_english) { // spawned
    assert(f->sync_hebrew);
    f->current_english = f->sync_english;
    f->current_hebrew = f->sync_hebrew;
    f->sync_english = NULL; f->sync_hebrew = NULL;
  } else { // function didn't spawn
    assert(!f->sync_english); assert(!f->sync_hebrew);
  }

}

extern "C" void cilk_leave_begin (__cilkrts_stack_frame *sf)
{ 
  if (__cilkrts_get_batch_id(sf->worker) != -1) return;

  if (frames[sf->worker->self].size() == 1) { // empty
    //    frames[sf->worker->self].pop();
  } else {
    frame_t type = frames[sf->worker->self].head()->type;
    frames[sf->worker->self].pop();
    if (type == HELPER) return;
    FrameData_t* current = frames[sf->worker->self].ancestor(1);
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

extern "C" void cilk_leave_end (void)
{
  // std::cout << __FUNCTION__ << std::endl;
}
