// A wrapper around my OM data structure, made specifically with the
// extra features necessary for race detection.
#include <internal/abi.h> // for __cilkrts_get_nworkers();
#include <cilk/batcher.h> // for cilK_batchify
#include <cstdio>
#include "rd.h"
#include "om/om.c"
#include "stack.h"
#include "debug_util.h"
#include "stat_time.h"

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

#define CAS(loc,old,nval) __sync_bool_compare_and_swap(loc,old,nval)

/// @todo break this into a FPSPController class (or something similar)
#include "rd.h" // for local_mut definition
local_mut* g_worker_mutexes;
pthread_spinlock_t g_relabel_mutex;

/// @todo either find a way to pass this through batchify or store it
/// in the OM DS.
__thread AtomicStack_t<tl_node*>* saved_heavy_nodes;
int g_batch_owner_id = -1;

// We actually only need the DS.
void batch_relabel(void* ds, void* data, size_t size, void* results)
{
  DBG_TRACE(DEBUG_BACKTRACE, "Begin relabel %zu.\n", g_num_relabels);
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

void join_batch(int self, om* ds, AtomicStack_t<tl_node*>& heavy_nodes)
{
  DBG_TRACE(DEBUG_BACKTRACE, "Worker %i calling batchify.\n", self);
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

#define HALF_BITS ((sizeof(label_t) * 8) / 2)
#define DEFAULT_HEAVY_THRESHOLD HALF_BITS
static label_t g_heavy_threshold = DEFAULT_HEAVY_THRESHOLD;

class omrd_t {
private:
  AtomicStack_t<tl_node*> m_heavy_nodes;
  om* m_ds;
  om_node* m_base;

  void add_heavy(om_node* base)
  {
    m_heavy_nodes.push();
    *m_heavy_nodes.head() = om_get_tl(base);
    assert(base->list->above->level == MAX_LEVEL);
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

  om_node* insert(__cilkrts_worker* w, om_node* base)
  {
    pthread_spinlock_t* mut = &g_worker_mutexes[w->self].mut;
    while (pthread_spin_trylock(mut) != 0) {
      join_batch(w->self, m_ds, m_heavy_nodes);
    }

    om_node* n = try_insert(base);
    while (!n) {
      if (!base->list->heavy && CAS(&base->list->heavy, 0, 1)) {
        DBG_TRACE(DEBUG_BACKTRACE,
                  "Could not fit into list of size %zu, not heavy.\n",
                  base->list->size);
        add_heavy(base);
      }

      pthread_spin_unlock(mut);
      join_batch(w->self, m_ds, m_heavy_nodes);
    
      while (pthread_spin_trylock(mut) != 0) {
        join_batch(w->self, m_ds, m_heavy_nodes);
      }
      n = try_insert(base);
    }
    assert(n->list == base->list);
    if (!n->list->heavy && count_set_bits(n->label) >= g_heavy_threshold) {
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
