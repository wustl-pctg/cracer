#include <stdio.h>
#include <stdlib.h> // realloc

#include <internal/abi.h>
#include "../om/om.h"

int g_tool_init = 0;
om* g_english;
om* g_hebrew;

size_t g_marked_array_size;
size_t g_marked_array_cap;
tl_node** g_marked_array;

#define HALF_BITS ((sizeof(label_t) * 8) / 2)

int get_all_locks()
{
  return 1;
}

void vector_insert_end(tl_node* n)
{
  /// Only works sequentially...
  if (g_marked_array_size == g_marked_array_cap) {
    g_marked_array = realloc(g_marked_array, g_marked_array_cap * 2);
    g_marked_array_cap += 2;
  }
  g_marked_array[g_marked_array_size] = n;
  g_marked_array_size++;
  /* size_t index = __sync_fetch_and_add(&g_marked_array_size, 1); */
  /* g_marked_array[index] = n->list->above; */
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

om_node* insert_or_relabel(__cilkrts_worker* w, om* ds, om_node* base)
{
  // Get worker om lock
  om_node* n = om_insert(ds, base);
  while (!n) {
    // release worker om lock
    // grab ALL worker om locks

    /// @todo don't call cilk_batchify
    /// call something else that doesn't do a collect
    //    cilk_batchify(om_relabel, 
    om_relabel(ds, g_marked_array, g_marked_array_size);
    n = om_insert(ds, base);
    // release all locks
  }
  if (count_set_bits(n->label) >= HALF_BITS) { // uses half available
                                               // bits
    vector_insert_end(om_get_tl(n));
  }
  return n;
}

void cilk_tool_init(void) 
{ 
  g_tool_init = 1;
  g_english = om_new();
  g_hebrew = om_new();

  g_marked_array_size = 0;
  g_marked_array_cap = 128;
  g_marked_array = calloc(g_marked_array_cap, sizeof(tl_node*));

  /// @todo allocate worker locks
}

// Currently, I don't think this is ever called. @todo
void cilk_tool_destroy(void) 
{ 
  om_free(g_english);
  om_free(g_hebrew);
}

void cilk_tool_print(void) { }

void cilk_tool_c_function_enter(void* this_fn, void* rip) { }
void cilk_tool_c_function_leave(void* rip) { }
                                
void cilk_enter_begin(__cilkrts_stack_frame* sf, void* this_fn, void* rip)
{
  if (sf->worker->batch_id != -1) return;
  if (!g_tool_init) cilk_tool_init();
}

void cilk_enter_helper_begin(__cilkrts_stack_frame* sf, void* this_fn, void* rip)
{ 
  if (sf->worker->batch_id != -1) return;
}

void cilk_enter_end (__cilkrts_stack_frame* sf, void* rsp)
{ 
  if (sf->worker->batch_id != -1) return;
}

void cilk_spawn_prepare (__cilkrts_stack_frame* sf)
{ 
  if (sf->worker->batch_id != -1) return;
}

void cilk_spawn_or_continue (int in_continuation) { }
void cilk_detach_begin (__cilkrts_stack_frame* parent)
{ 
  if (parent->worker->batch_id != -1) return;
}

void cilk_detach_end (void) { }
void cilk_sync_begin (__cilkrts_stack_frame* sf)
{ 
  if (sf->worker->batch_id != -1) return;
}

void cilk_sync_end (__cilkrts_stack_frame* sf)
{ 
  if (sf->worker->batch_id != -1) return;
}

void cilk_leave_begin (__cilkrts_stack_frame *sf)
{ 
  if (sf->worker->batch_id != -1) return;
}

void cilk_leave_end (void) { }
