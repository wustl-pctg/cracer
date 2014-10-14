/* local_state.h                  -*-C++-*-
 *
 *************************************************************************
 *
 *  @copyright
 *  Copyright (C) 2009-2011
 *  Intel Corporation
 *  
 *  @copyright
 *  This file is part of the Intel Cilk Plus Library.  This library is free
 *  software; you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 3, or (at your option)
 *  any later version.
 *  
 *  @copyright
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  @copyright
 *  Under Section 7 of GPL version 3, you are granted additional
 *  permissions described in the GCC Runtime Library Exception, version
 *  3.1, as published by the Free Software Foundation.
 *  
 *  @copyright
 *  You should have received a copy of the GNU General Public License and
 *  a copy of the GCC Runtime Library Exception along with this program;
 *  see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 *  <http://www.gnu.org/licenses/>.
 **************************************************************************/

/**
 * @file local_state.h
 *
 * @brief The local_state structure contains additional OS-independent
 * information that's associated with a worker, but doesn't need to be visible
 * to the code generated by the compiler.
 */

#ifndef INCLUDED_LOCAL_STATE_DOT_H
#define INCLUDED_LOCAL_STATE_DOT_H

#include <internal/abi.h>
#include "worker_mutex.h"
#include "global_state.h"
#include "record-replay.h"
#include "signal_node.h"

#include <setjmp.h>
#include <stddef.h>
#include <stdio.h>


#ifndef _WIN32
#   include <pthread.h>
#endif

__CILKRTS_BEGIN_EXTERN_C

/* Opaque types. */

struct full_frame;
struct free_list;
struct pending_exception_info;
/// Opaque type for replay entry. 
typedef struct replay_entry_t replay_entry_t;

/**
 * @brief Magic numbers for local_state, used for debugging
 */
typedef unsigned long long ls_magic_t;

/**
 * @brief Scheduling stack function: A function that is decided on the program stack,
 * but that must be executed on the scheduling stack.
 */
typedef void (*scheduling_stack_fcn_t) (__cilkrts_worker *w,
                                        struct full_frame *ff,
                                        __cilkrts_stack_frame *sf);

/**
 * @brief Type of this worker.
 **/
typedef enum cilk_worker_type
  {
    WORKER_FREE,    ///< Unused worker - available to be bound to user threads
    WORKER_SYSTEM,  ///< Worker created by runtime - able to steal from any worker
    WORKER_USER     ///< User thread - able to steal only from team members
  } cilk_worker_type;


/**
 * @brief The local_state structure contains additional OS-independent
 * information that's associated with a worker, but doesn't need to be
 * visible to the compiler.
 *
 * No compiler-generated code should need to know the layout of this
 * structure.
 *
 * The fields of this struct can be classified as either local or
 * shared.
 *
 *  Local: This field is only accessed by the thread bound to this
 *    worker struct.  Local fields can be freely accessed without
 *    acquiring locks.
 *  
 *  Shared: This field may be accessed by multiple worker threads.
 *    Accesses to shared fields usually requires locks, except in
 *    special situations where one can prove that locks are
 *    unnecessary.
 *
 * The fields of this can also be classified as "read-only" if the
 * field does not change after it is initialized.  Otherwise, the
 * field is "read/write".  Read-only fields do not require locks to
 * access (ignoring the synchronization that might be needed for
 * initialization if this can occur in parallel).
 *
 * Finally, we explicitly classify some fields as "synchronization"
 * fields if they are used as part of a synchronization protocol in
 * the runtime.  These variables are generally shared and read/write.
 * Mostly, this category includes lock variables and other variables
 * that are involved in synchronization protocols (i.e., the THE
 * protocol).
 */
struct local_state  /* COMMON_PORTABLE */
{
  /** This value should be in the first field in any local_state */
#   define WORKER_MAGIC_0 ((ls_magic_t)0xe0831a4a940c60b8ULL)

  /**
   * Should be WORKER_MAGIC_0 or the local_state has been corrupted
   * This magic field is shared because it is read on lock acquisitions.
   *
   * [shared read-only]
   */
  ls_magic_t worker_magic_0;

  /**
   * Mutex used to serialize access to the local_state
   * Synchronization field. [shared read/write]
   */
  struct mutex lock;

  /**
   * Flag that indicates that the worker is interested in grabbing
   * LOCK, and thus thieves should leave the worker alone.
   * Written only by self, may be read by others.
   *
   * Synchronization field.  [shared read/write]
   */
  int do_not_steal;

  /**
   * Lock that all thieves grab in order to compete for the right
   * to disturb this worker.
   *
   * Synchronization field. [shared read/write]
   */
  struct mutex steal_lock;

  /**
   * Full frame that the worker is working on.
   *
   * While a worker w is executing, a thief may change
   * w->l->frame_ff (on a successful steal) after acquiring w's
   * lock.
   *
   * Unlocked accesses to w->l->frame_ff are safe (by w itself) when
   * w's deque is empty, or when stealing from w has been disabled.
   *
   * [shared read/write]
   */
  struct full_frame *frame_ff;

  /**
   * Full frame that the worker will be working on next
   *
   * This field is normally local for a worker w.  Another worker v
   * may modify w->l->next_frame_ff, however, in the special case
   * when v is returning a frame to a user thread w since w is the
   * team leader.
   *
   * [shared read/write]
   */
  struct full_frame *next_frame_ff;

  /**
   * This is set iff this is a WORKER_USER and there has been a steal.  It
   * points to the first frame that was stolen since the team was last fully
   * sync'd.  Only this worker may continue past a sync in this function.
   *
   * This field is set by a thief for a victim that is a user
   * thread, while holding the victim's lock.
   * It can be cleared without a lock by the worker that will
   * continue exuecting past the sync.
   *
   * [shared read/write]
   */
  struct full_frame *last_full_frame;

  /**
   * Team on which this worker is a participant.  When a user worker enters,
   * its team is its own worker struct and it can never change teams.  When a
   * system worker steals, it adopts the team of its victim.
   *
   * When a system worker w steals, it reads victim->l->team and
   * joins this team.  w->l->team is constant until the next time w
   * returns control to the runtime.
   * We must acquire the worker lock to change w->l->team.
   *
   * @note This field is 64-byte aligned because it is the first in
   * the group of shared read-only fields.  We want this group to
   * fall on a different cache line from the previous group, which
   * is shared read-write.
   *
   * [shared read-only]
   */
  __attribute__((aligned(64)))
  __cilkrts_worker *team;

  /**
   * Type of this worker
   *
   * This field changes only when a worker binds or unbinds.
   * Otherwise, the field is read-only while the worker is bound.
   *
   * [shared read-only]
   */
  cilk_worker_type type;
    
  /**
   * Lazy task queue of this worker - an array of pointers to stack frames.
   *
   * Read-only because deques are a fixed size in the current
   * implementation.
   *
   * @note This field is 64-byte aligned because it is the first in
   * the group of local fields.  We want this group to fall on a
   * different cache line from the previous group, which is shared
   * read-only.
   *
   * [local read-only]
   */
  __attribute__((aligned(64)))
  __cilkrts_stack_frame **ltq;

  /**
   * Pool of fibers waiting to be reused.
   * [local read/write]
   */
  cilk_fiber_pool fiber_pool;

  /**
   * The fiber for the scheduling stacks.
   * [local read/write]
   */
  cilk_fiber* scheduling_fiber;

  /**
   * Just a temporary holder -- we want to save the core scheduling
   * fiber while use a new fiber for batch scheduling.
   * [local read/write]
   */
  cilk_fiber* core_scheduling_fiber;

  /**
   * Another temporary holder, this time for the user fiber that calls
   * batchify.
   * [local read/write]
   */
  cilk_fiber* core_user_fiber;
  cilk_fiber* batch_user_fiber;

  /**
   * The fiber for the batch scheduling stack.
   * I'm not actually sure if this is strictly necessary, but it seems
   * like the get_current_fiber function is not supported on all
   * platforms, so we'll just store it here.
   * [local read/write]
   */
   cilk_fiber* batch_scheduling_fiber;

  /**
   * Saved pointer to the leaf node in thread-local storage, when a
   * user thread is imported.  This pointer gets set to a
   * meaningful value when binding a user thread, and cleared on
   * unbind.
   *
   * [local read/write]
   */
  __cilkrts_pedigree* original_pedigree_leaf;

  /**
   * State of the random number generator
   * 
   * [local read/write]
   */
  unsigned rand_seed;

  /**
   * Function to execute after transferring onto the scheduling stack.
   *
   * [local read/write]
   */
  scheduling_stack_fcn_t post_suspend;

  /**
   * __cilkrts_stack_frame we suspended when we transferred onto the
   * scheduling stack.    
   *
   * [local read/write]
   */
  __cilkrts_stack_frame *suspended_stack;

  /**
   * cilk_fiber that should be freed after returning from a
   *  spawn with a stolen parent or after stalling at a sync.

   *  We calculate the stack to free when executing a reduction on
   *  the user stack, but we can not actually release the stack
   *  until control longjmps onto a runtime scheduling stack.
   *
   * This field is used to pass information to the runtime across
   * the longjmp onto the scheduling stack.
   *
   * [local read/write]
   */
  cilk_fiber* fiber_to_free;

  /**
   * Saved exception object for an exception that is being passed to
   * our parent
   *
   * [local read/write]
   */
  struct pending_exception_info *pending_exception;

  /**
   * Buckets for the memory allocator
   *
   * [local read/write]
   */
  struct free_list *free_list[FRAME_MALLOC_NBUCKETS];

  /**
   * Potential function for the memory allocator
   *
   * [local read/write]
   */
  size_t bucket_potential[FRAME_MALLOC_NBUCKETS];

  /**
   * Support for statistics
   *
   * Useful only when CILK_PROFIlE is compiled in. 
   * [local read/write]
   */
  statistics* stats;

  /**
   * Count indicates number of failures since last successful steal.  This is
   * used by the scheduler to reduce contention on shared flags.
   *
   * [local read/write]
   */
  unsigned int steal_failure_count;

  /**
   * 1 if work was stolen from another worker.  When true, this will flag
   * setup_for_execution_pedigree to increment the pedigree when we resume
   * execution to match the increment that would have been done on a return
   * from a spawn helper.
   *
   * [local read/write]
   */
  int work_stolen;

  /**
   * File pointer for record or replay
   * Does FILE * work on Windows?
   * During record, the file will be opened in write-only mode.
   * During replay, the file will be opened in read-only mode.
   *
   * [local read/write]
   */
  FILE *record_replay_fptr;

  /**
   * Root of array of replay entries - NULL if we're not replaying a log
   *
   * [local read/write]
   */
  replay_entry_t *replay_list_root;

  /**
   * Current replay entry - NULL if we're not replaying a log
   *
   * [local read/write]
   */
  replay_entry_t *replay_list_entry;
    
  /**
   * Separate the signal_node from other things in the local_state by the
   * sizeof a cache line for performance reasons.
   *
   * unused 
   */
  char buf[64];

  /**
   * Signal object for waking/sleeping the worker.  This should be a pointer
   * to avoid the possibility of caching problems.
   *
   * [shared read-only]
   */
  signal_node_t *signal_node;

  /** This value should be in the last field in any local_state */
#   define WORKER_MAGIC_1 ((ls_magic_t)0x16164afb0ea0dff9ULL)

  /**
   * Should be WORKER_MAGIC_1 or the local_state has been corrupted
   * This magic field is shared because it is read on lock acquisitions.
   * [shared read-only]
   */
  ls_magic_t worker_magic_1;
};

/**
 * Perform cleanup according to the function set before the longjmp().
 *
 * Call this after longjmp() has completed and the worker is back on a
 * scheduling stack.
 *
 * @param w __cilkrts_worker currently executing.
 */
void run_scheduling_stack_fcn(__cilkrts_worker *w);

__CILKRTS_END_EXTERN_C

#endif // ! defined(INCLUDED_LOCAL_STATE_DOT_H)
