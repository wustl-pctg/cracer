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

COMMON_PORTABLE
void internal_batchify(cilk_fiber *fiber)
{
  printf("In internal batchify\n");
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
	printf("Doing the function\n");
	printf("Getting cilk worker\n");
	__cilkrts_worker * w = __cilkrts_get_tls_worker();
  cilk_fiber *batch_fiber = NULL;
  cilk_fiber *current_fiber = w->l->frame_ff->fiber_self;

  printf("Allocating new fiber\n");
	START_INTERVAL(w, INTERVAL_FIBER_ALLOCATE) {
    batch_fiber = cilk_fiber_allocate(&w->l->fiber_pool); // allocate_from_thread? TODO(rob)
    cilk_fiber_reset_state(batch_fiber, internal_batchify);
    cilk_fiber_set_owner(batch_fiber, w);
  } STOP_INTERVAL(w, INTERVAL_FIBER_ALLOCATE);

  if (batch_fiber == NULL) {

    // Should manually try to get lock and run batch sequentially?
    printf("Could not get a fiber!\n");

  } else {
    /* full_frame *ff = __cilkrts_make_full_frame(w, 0); */
    /* ff->join_counter = 1; */
    /* w->l->frame_ff = ff; */
    /* f(); */

    cilk_fiber_suspend_self_and_resume_other(current_fiber, batch_fiber);
  }

  // suspend_self_and_resume_other
}
