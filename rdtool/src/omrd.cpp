// A wrapper around my OM data structure, made specifically with the
// extra features necessary for race detection.
#include <internal/abi.h> // for __cilkrts_get_nworkers();
#include <cilk/batcher.h> // for cilK_batchify
#include <cstdio>
#include "rd.h"
#include "om/om.c"
#include "stack.h"
#include "debug_util.h"
#include "stat_util.h"

#define HALF_BITS ((sizeof(label_t) * 8) / 2)
#define DEFAULT_HEAVY_THRESHOLD HALF_BITS
static label_t g_heavy_threshold = DEFAULT_HEAVY_THRESHOLD;

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

//int is_heavy(om_node* n) { return count_set_bits >= g_heavy_threshold; }
int is_heavy(om_node* n)
{
  label_t next_lab = (n->next) ? n->next->label : MAX_LABEL;
  return (next_lab - n->label) < (1L << HALF_BITS);
}

#define CAS(loc,old,nval) __sync_bool_compare_and_swap(loc,old,nval)
void batch_relabel(void* _ds, void* data, size_t size, void* results);

/// @todo break this into a FPSPController class (or something similar)
#include "rd.h" // for local_mut definition
local_mut* g_worker_mutexes;
pthread_spinlock_t g_relabel_mutex;

int g_batch_owner_id = -1;

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

class omrd_t {
private:
  AtomicStack_t<tl_node*> m_heavy_nodes;
  om* m_ds;
  om_node* m_base;

  void add_heavy(om_node* base)
  {
    m_heavy_nodes.add(om_get_tl(base));
    om_assert(base->list->above->level == MAX_LEVEL);
  }

  om_node* try_insert(om_node* base = NULL)
  {
    om_node* n;
    if (base == NULL) n = om_insert_initial(m_ds);
    else n = om_insert(m_ds, base);

    if (base) om_assert(base->list->above);
    if (n) om_assert(n->list->above);
    return n;
  }

  void join_batch(int self)
  {
    DBG_TRACE(DEBUG_BACKTRACE, "Worker %i calling batchify.\n", self);
    cilk_batchify(batch_relabel, this, 0, sizeof(int));
    if (self == g_batch_owner_id) {
      g_batch_owner_id = -1;
      for (int i = 0; i < __cilkrts_get_nworkers(); ++i) {
        pthread_spin_unlock(&g_worker_mutexes[i].mut);
      }
      pthread_spin_unlock(&g_relabel_mutex);
    }
  }

  om_node* slow_path(__cilkrts_worker *w, om_node* base)
  {
    RDTOOL_INTERVAL_BEGIN(SLOW_PATH);
    om_node *n = NULL;
    pthread_spinlock_t* mut = &g_worker_mutexes[w->self].mut;
    while (!n) {
      if (!base->list->heavy && CAS(&base->list->heavy, 0, 1)) {
        DBG_TRACE(DEBUG_BACKTRACE,
                  "Could not fit into list of size %zu, not heavy.\n",
                  base->list->size);
        add_heavy(base);
      }

      pthread_spin_unlock(mut);
      join_batch(w->self);
    
      while (pthread_spin_trylock(mut) != 0) {
        join_batch(w->self);
      }
      n = try_insert(base);
    }
    RDTOOL_INTERVAL_END(SLOW_PATH);
    return n;
  }

public:

  omrd_t()
  {
    m_ds = om_new();
    m_base = try_insert();
  }

  ~omrd_t()
  {
    om_free(m_ds);
  }

  om_node* get_base() { return m_base; }
  AtomicStack_t<tl_node*>* get_heavy_nodes() { return &m_heavy_nodes; }
  om* get_ds() { return m_ds; }

  om_node* insert(__cilkrts_worker* w, om_node* base)
  {
    RDTOOL_INTERVAL_BEGIN(FAST_PATH);
    pthread_spinlock_t* mut = &g_worker_mutexes[w->self].mut;
    while (pthread_spin_trylock(mut) != 0) {
      join_batch(w->self);
    }

    om_node* n = try_insert(base);
    RDTOOL_INTERVAL_END(FAST_PATH);
    if (!n) n = slow_path(w, base);
    assert(n->list == base->list);
    if (!n->list->heavy && is_heavy(n)) {
      if (CAS(&base->list->heavy, 0, 1)) {
        DBG_TRACE(DEBUG_BACKTRACE,
                  "List marked as heavy with %zu items.\n", n->list->size);
        add_heavy(base);
      }
    }
    pthread_spin_unlock(mut);
    RD_STATS(__sync_fetch_and_add(&g_num_inserts, 1));
    return n;
  }


  label_t set_heavy_threshold(label_t new_threshold)
  {
    label_t old = g_heavy_threshold;
    g_heavy_threshold = new_threshold;
    return old;
  }
};

// We actually only need the DS.
/// @todo It would be great define batch functions as class methods...
void batch_relabel(void* _ds, void* data, size_t size, void* results)
{
  DBG_TRACE(DEBUG_BACKTRACE, "Begin relabel %zu.\n", g_num_relabels);
  omrd_t* ds = (omrd_t*)_ds;
  AtomicStack_t<tl_node*>* heavy_nodes = ds->get_heavy_nodes();
  if (!heavy_nodes->empty()) {
    om_relabel(ds->get_ds(), heavy_nodes->at(0), heavy_nodes->size());
  }

  RD_STATS(g_num_relabels++);
  RD_STATS(g_relabel_size += heavy_nodes->size());

  heavy_nodes->reset();
}
