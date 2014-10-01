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

CILK_API_VOID cilk_batchify(function_t f)
{
	printf("Doing the function");
	printf("Getting cilk worker");
	__cilkrts_worker * w = __cilkrts_get_tls_worker();
    cilk_fiber *fiber = NULL;
    printf("Allocating new fiber");
	START_INTERVAL(w, INTERVAL_FIBER_ALLOCATE) {
        fiber = cilk_fiber_allocate(&w->l->fiber_pool);
    } STOP_INTERVAL(w, INTERVAL_FIBER_ALLOCATE);
    if (fiber == NULL) {
    	printf("Could not get a fiber!");
    } else {
        full_frame *ff = __cilkrts_make_full_frame(w, 0);
        ff->join_counter = 1;
        w->l->frame_ff = ff;
        f();
    }
}
