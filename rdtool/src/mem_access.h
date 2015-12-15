/* -*- Mode: C++ -*- */

#ifndef _MEM_ACCESS_H
#define _MEM_ACCESS_H

#include <stdio.h>
#include <iostream>
#include <inttypes.h>
#include <pthread.h>

#include "debug_util.h"
#include "rd.h" // is this necessary?
#include "om/om.h"

#define MAX_GRAIN_SIZE 8
// a mask that keeps all the bits set except for the least significant bits
// that represent the max grain size
#define MAX_GRAIN_MASK (~(uint64_t)(MAX_GRAIN_SIZE-1))

// If the value is already divisible by MAX_GRAIN_SIZE, return the value; 
// otherwise return the previous / next value divisible by MAX_GRAIN_SIZE.
#define ALIGN_BY_PREV_MAX_GRAIN_SIZE(addr) ((uint64_t) (addr & MAX_GRAIN_MASK))
#define ALIGN_BY_NEXT_MAX_GRAIN_SIZE(addr) \
  ((uint64_t) ((addr+(MAX_GRAIN_SIZE-1)) & MAX_GRAIN_MASK))

enum GrainType_t { UNINIT = -1, ONE = 0, TWO = 1, FOUR = 2, EIGHT = 3 };
static const int gtype_to_mem_size[4] = { 1, 2, 4, 8 };
#define MAX_GTYPE EIGHT // the max value that the enum GrainType_t can take

// check if addr is aligned with the granularity represented by gtype
#define IS_ALIGNED_WITH_GTYPE(addr, gtype) \
  ((addr & (uint64_t)gtype_to_mem_size[gtype]-1) == 0)

//extern pthread_spinlock_t* g_worker_mutexes;
extern local_mut* g_worker_mutexes;
extern __thread int self;

// Struct to hold strands corresponding to left / right readers and last writer
typedef struct MemAccess_t {

  om_node *estrand; // the strand that made this access stored in g_english
  om_node *hstrand; // the strand that made this access stored in g_hebrew
  uint64_t rip; // the instruction address of this access
  int16_t ref_count; // number of pointers aliasing to this object
  // ref_count == 0 if only a single unique pointer to this object exists

  MemAccess_t(om_node *_estrand, om_node *_hstrand, uint64_t _rip)
    : estrand(_estrand), hstrand(_hstrand), rip(_rip), ref_count(0)
  { }

  inline bool races_with(om_node *curr_estrand, om_node *curr_hstrand) {
    om_assert(estrand); om_assert(hstrand);
    om_assert(curr_estrand); om_assert(curr_hstrand);

    /// @todo Is it worth it to join the batch relabel here?
    om_assert(self > -1);
    pthread_spin_lock(&g_worker_mutexes[self].mut);
    bool prec_in_english = om_precedes(estrand, curr_estrand);
    bool prec_in_hebrew  = om_precedes(hstrand, curr_hstrand);
    pthread_spin_unlock(&g_worker_mutexes[self].mut);

    // race if the ordering in english and hebrew differ
    bool has_race = (prec_in_english == !prec_in_hebrew);
#if OM_DEBUG > DEBUG_BASIC
    if(has_race) {
      DBG_TRACE(DEBUG_MEMORY, 
            "Race with estrand: %p, and curr estrand: %p, prec: %d.\n", 
            estrand, curr_estrand, prec_in_english);
      DBG_TRACE(DEBUG_MEMORY, 
            "Race with hstrand: %p, and curr hstrand: %p, prec: %d.\n", 
            hstrand, curr_hstrand, prec_in_hebrew);
    }
#endif

    return has_race;
  }

  inline int16_t inc_ref_count() { ref_count++; return ref_count; }
  inline int16_t dec_ref_count() { ref_count--; return ref_count; }

} MemAccess_t;


class MemAccessList_t {

private:
  // the smallest addr of memory locations that this MemAccessList represents
  uint64_t start_addr;
  enum GrainType_t lreader_gtype; // for left-most readers
  MemAccess_t *lreaders[MAX_GRAIN_SIZE];
  enum GrainType_t rreader_gtype; // for right-most readers
  MemAccess_t *rreaders[MAX_GRAIN_SIZE];
  enum GrainType_t writer_gtype;
  MemAccess_t *writers[MAX_GRAIN_SIZE];

  // locks for updating lreaders, rreaders, and writers
  pthread_spinlock_t lreader_lock;
  pthread_spinlock_t rreader_lock;
  pthread_spinlock_t writer_lock;

  __attribute__((always_inline))
  static enum GrainType_t mem_size_to_gtype(size_t size) {
    om_assert(size > 0 && size <= MAX_GRAIN_SIZE);
    switch(size) {
      case 8:
        return EIGHT;
      case 4:
        return FOUR;
      case 2:
        return TWO;
      default: // everything else gets byte-granularity
        return ONE;
    }
  }

  __attribute__((always_inline))
  static int get_prev_aligned_index(int index, enum GrainType_t gtype) {

    om_assert(index >= 0 && index < MAX_GRAIN_SIZE);
    om_assert(gtype != UNINIT && gtype <= MAX_GTYPE);
    if( IS_ALIGNED_WITH_GTYPE(index, gtype) ) {
      return index;
    }
    return ((index >> gtype) << gtype);
  }

  // get the start and end indices and gtype to use for accesing 
  // the readers / writers lists; the gtype is the largest granularity
  // that this memory access is aligned with 
  static enum GrainType_t 
  get_mem_index(uint64_t addr, size_t size, int& start, int& end); 

  // helper function: break one of the MemAcess list into a smaller
  // granularity;
  void break_list_into_smaller_gtype(MemAccess_t **l, 
                                     enum GrainType_t *old_gtype, 
                                     enum GrainType_t new_gtype) {

    om_assert(*old_gtype > new_gtype && new_gtype != UNINIT);
    const int stride = gtype_to_mem_size[new_gtype];
    MemAccess_t *acc = l[0];

    for(int i = stride; i < MAX_GRAIN_SIZE; i += stride) {
      if( IS_ALIGNED_WITH_GTYPE(i, *old_gtype) ) { 
        acc = l[i];
      } else if(acc) {
        om_assert(l[i] == NULL);
        acc->inc_ref_count();
        l[i] = acc;
      }
    }
    *old_gtype = new_gtype;
  }

  // Check races on memory represented by this mem list with this read access
  // Once done checking, update the mem list with this new read access
  void check_races_and_update_with_read(uint64_t inst_addr, uint64_t addr,
                              size_t mem_size, 
                              om_node *curr_estrand, om_node *curr_hstrand);
  
  // Check races on memory represented by this mem list with this write access
  // Also, update the writers list.  
  // Very similar to check_races_and_update_with_read function above.
  void check_races_and_update_with_write(uint64_t inst_addr, uint64_t addr, 
                              size_t mem_size,
                              om_node *curr_estrand, om_node *curr_hstrand);

public:

  // Constructor.
  //
  // addr: the memory address of the access
  // is_read: whether the initializing memory access is a read
  // acc: the memory access that causes this MemAccessList_t to be created
  // mem_size: the size of the access 
  MemAccessList_t(uint64_t addr, bool is_read,
                  MemAccess_t *acc, size_t mem_size); 

  ~MemAccessList_t();


  // Check races on memory represented by this mem list with this mem access
  // Once done checking, update the mem list with the new mem access
  //
  // is_read: whether this access is a read or not
  // in_user_context: whether this access is made by user strand or runtime
  //                  strand (e.g., update / reduce)
  // inst_addr: the instruction that performs the read
  // addr: the actual memory location accessed
  // mem_size: the size of this memory access
  // curr_estrand: the strand stored in g_english corresponding to this access
  // curr_hstrand: the strand stored in g_hebrew corresponding to this access
  __attribute__((always_inline))
  void
  check_races_and_update(bool is_read, uint64_t inst_addr, uint64_t addr,
                         size_t mem_size,
                         om_node *curr_estrand, om_node *curr_hstrand) {
    if(is_read) {
      check_races_and_update_with_read(inst_addr, addr, mem_size,
                                       curr_estrand, curr_hstrand);
    } else {
      check_races_and_update_with_write(inst_addr, addr, mem_size,
                                        curr_estrand, curr_hstrand);
    }
  }

#if OM_DEBUG > DEBUG_BASIC
  void check_invariants(uint64_t current_func_id); 
#endif
}; // end of class MemAccessList_t def

#endif // _MEM_ACCESS_H
