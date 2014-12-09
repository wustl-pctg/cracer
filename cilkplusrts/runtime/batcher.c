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
#include "os.h" // For __cilkrts_short_pause()

#include <stdio.h>

// @TODO?
//cilk_fiber_sysdep* cilkos_get_tls_cilk_fiber(void)

// @TODO @HACK why are these not just defined in scheduler.h?
#define BEGIN_WITH_WORKER_LOCK(w) __cilkrts_worker_lock(w); do
#define END_WITH_WORKER_LOCK(w)   while (__cilkrts_worker_unlock(w), 0)
#define BEGIN_WITH_FRAME_LOCK(w, ff)                                     \
    do { full_frame *_locked_ff = ff; __cilkrts_frame_lock(w, _locked_ff); do

#define END_WITH_FRAME_LOCK(w, ff)                       \
    while (__cilkrts_frame_unlock(w, _locked_ff), 0); } while (0)


function_t batchFunc; // @TODO(rob) move

void scheduling_fiber_prepare_to_resume_user_code(__cilkrts_worker *w,
                                                  full_frame *ff,
                                                  __cilkrts_stack_frame *sf);

COMMON_PORTABLE
struct batch* create_batch(struct batch_record* record)
{
  __cilkrts_worker* w = __cilkrts_get_tls_worker();
  struct batch* b = &w->g->pending_batch;
  struct batch_record* records = w->g->batch_records;

  b->id++;
  b->operation = record->operation;
  b->ds = record->ds;
  b->data_size = record->data_size;
  b->num_ops = collect_batch(b, records);

  return b;
}

COMMON_PORTABLE
int collect_batch(struct batch* pending, struct batch_record* records)
{
  int num_ops = 0;
  int p = cilkg_get_nworkers();

  for (int i = 0; i < p; ++i) {
    if (ITEM_WAITING == records[i].status &&
        pending->operation == records[i].operation) {

      records[i].status = ITEM_IN_PROGRESS;
      memcpy(pending->work_array + num_ops, &records[i].data, pending->data_size);

      num_ops++;
    }
  }
  return num_ops;
}

COMMON_PORTABLE
void terminate_batch(struct batch_record* records)
{
  __cilkrts_worker* w = __cilkrts_get_tls_worker();
  int p = cilkg_get_nworkers();

  for (int i = 0; i < p; ++i) {
    if (records[i].status == ITEM_IN_PROGRESS)
      records[i].status = ITEM_DONE;
  }

  //  __cilkrts_mutex_unlock(w, &w->g->batch_lock);
  // Special unlock here because this worker may not be the original owner!
  w->g->batch_lock.owner = 0;
  __sync_lock_release(&w->g->batch_lock.lock);
  return;
}

COMMON_PORTABLE
void invoke_batch(cilk_fiber *fiber)
{
	__cilkrts_worker * w = __cilkrts_get_tls_worker();

  cilk_fiber* current_fiber = w->l->batch_user_fiber;
  //  cilk_fiber *current_fiber = w->l->frame_ff->fiber_self;

  // 1. Make sure deque (ltq) pointers are correctly set.

  // 2. Do the collection for the batch.
  struct batch_record* records = w->g->batch_records;
  struct batch* b = &w->g->pending_batch;
  create_batch(&records[w->self]);

  // 3. Call the batch operation.
  printf("Worker %i invoking batch %i.\n", w->self, b->id);
  full_frame* ff = __cilkrts_make_full_frame(w, 0);
  ff->fiber_self = current_fiber;
  cilk_fiber_set_owner(ff->fiber_self, w);
  ff->join_counter = 1;
  w->l->frame_ff = ff;

  batchFunc();

  // Note: SOMEONE will return back here, even if it is not the
  // original batch owner.
  // 4. Terminate the batch.
  terminate_batch(records);
  __cilkrts_destroy_full_frame(w, ff);
  printf("Worker %i terminated batch %i.\n", w->self, b->id);

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
  cilk_fiber *current_fiber = w->l->batch_scheduling_fiber;
  full_frame* ff = NULL;

  printf("Worker %i entered scheduler function at batch %i.\n", w->self, w->g->pending_batch.id);

  // @TODO(rob) interval statistics
  while (w->g->batch_records[w->self].status != ITEM_DONE) {
    //    __cilkrts_short_pause();

    /* cilk_fiber* fiber_to_resume */
    /*   = worker_scheduling_loop_body(w->l->batch_scheduling_fiber, w); */

    ff = pop_next_frame(w);
    if (!ff) ff = check_for_work(w);
    if (ff) {
      printf("Worker %i stole a batch frame!\n", w->self);
      __cilkrts_stack_frame* sf;
      BEGIN_WITH_WORKER_LOCK(w) {
        CILK_ASSERT(!w->l->frame_ff);
        BEGIN_WITH_FRAME_LOCK(w, ff) {
          sf = ff->call_stack;
          CILK_ASSERT(sf && !sf->call_parent);
          setup_for_execution(w, ff, 0);
        } END_WITH_FRAME_LOCK(w, ff);
      } END_WITH_WORKER_LOCK(w);

      scheduling_fiber_prepare_to_resume_user_code(w, ff, sf);
      
      cilk_fiber *other = w->l->frame_ff->fiber_self;
      cilk_fiber_data* other_data = cilk_fiber_get_data(other);
      cilk_fiber_data* current_fiber_data = cilk_fiber_get_data(current_fiber);

      CILK_ASSERT(NULL == other_data->resume_sf);

      current_fiber_data->resume_sf = NULL;
      CILK_ASSERT(current_fiber_data->owner == w);
      other_data->resume_sf = sf;

      cilk_fiber_suspend_self_and_resume_other(w->l->batch_scheduling_fiber,
                                               other);
    }

  }

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
  cilk_fiber *current_fiber;
  cilk_fiber *batch_fiber = NULL;
  __cilkrts_worker* saved_team = NULL;
  cilk_worker_type saved_type;

  // @TODO split out "switch to batch" function
  // Get own lock and save deque pointers
  BEGIN_WITH_WORKER_LOCK(w) {

    current_fiber = w->l->frame_ff->fiber_self;
    saved_team = w->l->team;
    saved_type = w->l->type;
    w->l->team = (w->l->type == WORKER_USER) ? w : NULL;
    w->l->type = WORKER_SYSTEM;

    w->l->saved_tail = w->tail;
    w->l->saved_head = w->head;
    w->l->saved_exc = w->exc;
    w->l->saved_protected_tail = w->protected_tail;

    w->head = w->tail = w->exc = w->l->batch_ltq;
    w->protected_tail = w->l->batch_ltq + w->g->ltqsize;
    //    __cilkrts_restore_stealing(w, w->ltq_limit);
    w->l->saved_frame_ff = w->l->frame_ff;
    w->l->frame_ff = NULL;
    __cilkrts_fence();
  } END_WITH_WORKER_LOCK(w);

  struct batch_record* record = &w->g->batch_records[w->self];
  record->operation = NULL;
  record->ds = NULL;
  record->data_size = sizeof(int);
  record->data = 42;
  record->status = ITEM_WAITING;

  cilk_fiber *batch_scheduling_fiber = allocate_batch_fiber(w, &w->l->fiber_pool,
                                                            batch_scheduler_function);

  // Will need to assert this, but disabled now because I don't
  //  initially set it to NULL at initialization, yet.
  //  CILK_ASSERT(w->l->core_scheduling_fiber == NULL);

  w->l->core_user_fiber = current_fiber;
  w->l->core_scheduling_fiber = w->l->scheduling_fiber;
  w->l->scheduling_fiber = batch_scheduling_fiber;
  w->l->batch_scheduling_fiber = batch_scheduling_fiber;

  int is_batch_owner = __cilkrts_mutex_trylock(w, &w->g->batch_lock);

  if (is_batch_owner) {
    batch_fiber = allocate_batch_fiber(w, &w->l->fiber_pool,
                                       invoke_batch);
    w->l->batch_user_fiber = batch_fiber;
    batchFunc = f;
    printf("Worker %i starting a batch.\n", w->self);
  } else {
    batch_fiber = batch_scheduling_fiber;
    printf("Worker %i helping in a batch.\n", w->self);
  }


  cilk_fiber_suspend_self_and_resume_other(current_fiber, batch_fiber);

  BEGIN_WITH_WORKER_LOCK(w) {
    w->tail = w->l->saved_tail;
    w->head = w->l->saved_head;
    w->exc = w->l->saved_exc;
    w->protected_tail = w->l->saved_protected_tail;
    w->l->frame_ff = w->l->saved_frame_ff;

    w->l->team = saved_team;
    w->l->type = saved_type;

    w->l->saved_head = w->l->saved_tail = w->l->saved_exc = NULL;
    w->l->saved_protected_tail = NULL;
  } END_WITH_WORKER_LOCK(w);

  w->l->scheduling_fiber = w->l->core_scheduling_fiber;
  w->l->core_scheduling_fiber = NULL;
  w->l->core_user_fiber = NULL;
  w->l->batch_scheduling_fiber = NULL;
  w->l->batch_user_fiber = NULL;
}
