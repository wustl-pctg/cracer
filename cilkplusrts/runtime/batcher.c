/** @file batcher.c               -*-C-*-
 * @author Robert Utterback
 * @author Tom Wilkinson
 *
 * @brief The heart of the Batcher runtime system.
 *
 */

#include <cilk/batcher.h>
//#include "rts-common.h"
#include "scheduler.h"
#include "cilk_fiber.h"
#include "full_frame.h"
#include "stats.h"
#include "local_state.h"

#include <stdio.h>

function_t batchFunc;

COMMON_PORTABLE
void invoke_batch(cilk_fiber *fiber)
{
	__cilkrts_worker * w = __cilkrts_get_tls_worker();

  // NB: This may not actually be the current fiber! @TODO(rob)
  cilk_fiber* current_fiber = w->l->batch_user_fiber;
  //  cilk_fiber *current_fiber = w->l->frame_ff->fiber_self;

  // 1. Make sure deque (ltq) pointers are correctly set.

  // 2. Do the collection for the batch.

  // 3. Call the batch operation.
  batchFunc();

  // Note: SOMEONE will return back here, even if it is not the
  // original batch owner.
  // 4. Terminate the batch.

  // 5. Go back to the user code before the call to batchify.

  // @TODO(rob) May be slightly faster to just manually destroy the
  // batch scheduling fiber and jump back to the user fiber, rather
  // than jumping to it first. But I doubt it will make a huge difference.
  cilk_fiber_remove_reference_from_self_and_resume_other(current_fiber,
                                                         &w->l->fiber_pool,
                                                         w->l->batch_scheduling_fiber);
}

COMMON_PORTABLE
void batch_scheduler_function(cilk_fiber *fiber)
{
	__cilkrts_worker * w = __cilkrts_get_tls_worker();
  cilk_fiber *current_fiber = w->l->frame_ff->fiber_self;

  cilk_fiber_remove_reference_from_self_and_resume_other(w->l->batch_scheduling_fiber,
                                                         &w->l->fiber_pool,
                                                         w->l->core_user_fiber);

}

static cilk_fiber* allocate_batch_fiber(__cilkrts_worker* w,
                                        cilk_fiber_pool* pool,
                                        cilk_fiber_proc start_proc)
{
  cilk_fiber* batch_fiber;
	START_INTERVAL(w, INTERVAL_FIBER_ALLOCATE) {
    // allocate_from_thread? @TODO(rob)
    batch_fiber = cilk_fiber_allocate(pool); 
    cilk_fiber_reset_state(batch_fiber, start_proc);
    cilk_fiber_set_owner(batch_fiber, w);
  } STOP_INTERVAL(w, INTERVAL_FIBER_ALLOCATE);

  if (batch_fiber == NULL) {
    // Should manually try to get lock and run batch sequentially?
    __cilkrts_bug("Couldn't get a batch fiber.");
  }

  return batch_fiber;
}

/* Here's the plan for batchify:
 *  Every worker allocated a new fiber, set to enter a batch
 *  scheduling loop. The old fiber is set as the new "scheduling" fiber.
 *  At the end of this batch scheduler, we suspend that fiber (destroy
 *  it), and jump back to this fiber, where we return back to user
 *  code.

 * The batch owner must do something slightly different. There are two
 * options:
 *  1. Allocate an additional fiber. Jump to this fiber and execute an
 * "internal batchify" function which just calls the batch operation
 * (could also do collection and termination in there). That fiber is
 * then destroyed and we jump to the batch scheduling fiber, where we
 * just immediately destroy that and return back to user code.
 *
 * 2. Another option is to not allocate an additional fiber, and just
 * manually call the internal batchify operation. This avoids the
 * fiber overhead, but then we must do more careful accounting. In
 * particular, I would be a little concerned about the fiber running
 * the batch sharing a stack with a worker that stole something from
 * the batch owner's core deque. Although it may not actually be
 * anything to worry about.
 *
 * I think I'll implement #1 first, because it seems more straightforward.
 */
CILK_API_VOID cilk_batchify(function_t f)
{
	__cilkrts_worker * w = __cilkrts_get_tls_worker();
  cilk_fiber *current_fiber = w->l->frame_ff->fiber_self;
  cilk_fiber *batch_fiber = NULL;

  cilk_fiber *batch_scheduling_fiber = allocate_batch_fiber(w, &w->l->fiber_pool,
                                                            batch_scheduler_function);

  // Will need to assert this, but disabled now because I don't
  //  initially set it to NULL at initialization, yet.
  //  CILK_ASSERT(w->l->core_scheduling_fiber == NULL);

  w->l->core_user_fiber = current_fiber;
  w->l->core_scheduling_fiber = w->l->scheduling_fiber;
  w->l->scheduling_fiber = batch_scheduling_fiber;
  w->l->batch_scheduling_fiber = batch_scheduling_fiber;

  // @TODO(rob) Try to get the batch lock.
  int is_batch_owner = 1;
  if (is_batch_owner) {
    batch_fiber = allocate_batch_fiber(w, &w->l->fiber_pool,
                                       invoke_batch);
    w->l->batch_user_fiber = batch_fiber;
    batchFunc = f;
  } else {
    batch_fiber = batch_scheduling_fiber;
  }

  cilk_fiber_suspend_self_and_resume_other(current_fiber, batch_fiber);

  w->l->scheduling_fiber = w->l->core_scheduling_fiber;
  w->l->core_scheduling_fiber = NULL;
  w->l->core_user_fiber = NULL;
  w->l->batch_scheduling_fiber = NULL;
  w->l->batch_user_fiber = NULL;
}
