#ifndef _CILK_INVOKE_BATCH_C
#define _CILK_INVOKE_BATCH_C
#include <string.h>
#include <cilk.h>
#include <cilk-cilk2c-pre.h>
#include <cilk-cilk2c.h>

/************************************************************
 * Batcher-related routines
 ************************************************************/
/* Remove closure for frame f from bottom of pn's deque, but don't
   free the memory. The only reason I know of to do this is for the
   batch closures and frames, which we don't want to have to malloc
   and free every single time we start a batch. */
void Cilk_remove_and_keep_closure_and_frame(CilkWorkerState *const ws,
																						CilkStackFrame *f, int pn,
																						ReadyDeque *const deque_pool)
{
	Closure *t;

	deque_lock(ws, pn, deque_pool);
	t = deque_xtract_bottom(ws, pn, deque_pool);
	CILK_ASSERT(ws, t->frame == f);
	deque_unlock(ws, pn, deque_pool);
}

static inline set_batch_info(Batch* pending, InternalBatchOperation op,
                             size_t size, size_t data_size, void* ds)
{
  pending->operation = op;
  pending->size = size;
  pending->data_size = data_size;
  pending->data_structure = ds;
}

// @todo Must also compact the array of continuations.
static inline unsigned int compact(CilkWorkerState *const ws, Batch *pending,
                                   int *work_array, unsigned int num_spots)
{
  unsigned int num_ops = 0;
  unsigned int register i,j;

  InternalBatchOperation op = NULL;
  size_t data_size = 0;
  void* ds = NULL;

  Cilk_enter_state(ws, STATE_BATCH_COMPACTION);

  for (i = 0; i < USE_PARAMETER(active_size); i++) {

    // @todo @features Should also check that the op matches, for
    // multiple batch operations.
    if (pending->array[i].status == DS_WAITING) {

      pending->array[i].status = DS_IN_PROGRESS;
      pending->array[i].packed_index = num_ops;

      if (op == NULL) {
        op = pending->array[i].operation;
        data_size = pending->array[i].size;
        ds = pending->array[i].data_structure;
      }

      // @todo @feature Use memcpy here (or other) to support any data
      // type.
      for (j = 0; j < USE_PARAMETER(batchvals); j++) {
        work_array[num_ops + j] = ((int*)pending->array[i].args)[j];
      }

      num_ops += USE_PARAMETER(batchvals);
    }
  }

  CILK_ASSERT(ws, ds != NULL);
  CILK_ASSERT(ws, op != NULL);
  set_batch_info(pending, op, num_ops, data_size, ds);

#if CILK_STATS
  USE_SHARED(num_batches)++;
  USE_SHARED(batch_sizes)[(num_ops / USE_PARAMETER(batchvals)) - 1]++;
#endif

  Cilk_exit_state(ws, STATE_BATCH_COMPACTION);
  return num_ops;
}

static inline void Cilk_terminate_batch(CilkWorkerState *const ws)
{
  int i, index;
	//	Cilk_enter_state(ws, STATE_BATCH_TERMINATE);
	Batch* current = &USE_SHARED(pending_batch);
  Closure* t;
  //	void* results = USE_SHARED(batch_work_array);
  //	size_t dataSize = current->dataSize;

  for (i = 0; i < USE_PARAMETER(active_size); i++) {
    if (current->array[i].status == DS_IN_PROGRESS) {
			//			index = current->array[i].packedIndex;
			/* if (current->array[i].result) { */
			/* 	memcpy(current->array[i].result, &results[index], dataSize); */
			/* } */
      //      printf("Spot %i is done for batch %i.\n", i, ws->batch_id);
			current->array[i].status = DS_DONE;
		}
    t = current->array[i].continuation;

    /// @todo Sequentially spawning off for now...
    if (t) {
      deque_lock(ws, ws->self, USE_PARAMETER(deques));
      deque_add_bottom(ws, t, ws->self, USE_PARAMETER(deques));
      deque_unlock(ws, ws->self, USE_PARAMETER(deques));
    }
	}

  USE_SHARED(current_batch_id)++; // signal end of this batch
  USE_SHARED(batch_owner) = -1;
  __sync_lock_release(&USE_SHARED(batch_lock));
	//	Cilk_exit_state(ws, STATE_BATCH_TERMINATE);

  // @todo Now (somehow) spawn off all the continuations.
  // To reduce the span, a parallel_for loop should be used.
  // But for now it might be okay just to put everything on the deque.
  /* t = USE_SHARED(batch_continuation_array); */
  /* while (t != NULL) { */
  /*   deque_lock(ws, ws->self, USE_PARAMETER(deques)); */
  /*   deque_add_bottom(ws, t, ws->self, USE_PARAMETER(deques)); */
  /*   deque_unlock(ws, ws->self, USE_PARAMETER(deques)); */
  /*   t++; */
  /* } */
}

// @todo @refactor I don't think this actually helps anything. We
// still have to put an initial closure on the deque
// (setup_for_execution).
// Whether we put invoke_batch_slow on initially or the actual batch
// function doesn't particularly matter -- someone stealing
// invoke_batch will see that it is executing and just suspend and
// steal again.
// NB: I *think* this is what happens. I should check empirically just
// to be safe.
static void invoke_batch(CilkWorkerState* const _cilk_ws, void* ds,
                         InternalBatchOperation op,
                         unsigned int num)
{
  CilkWorkerState* const ws = _cilk_ws;
  unsigned int num_ops = num;
  int* work_array;

  Closure* cl = USE_PARAMETER(invoke_batch);
  deque_lock(ws, ws->self, USE_PARAMETER(ds_deques));
  Closure_lock(ws, cl);
  BatchFrame* f = USE_SHARED(batch_frame);

  // Don't execute the op twice, so signal here to never do this
  // again.
  f->header.entry = 1;

  setup_for_execution(ws, cl);
  Closure_unlock(ws, cl);
  deque_add_bottom(ws, cl, ws->self, USE_PARAMETER(ds_deques));
  deque_unlock(ws, ws->self, USE_PARAMETER(ds_deques));

  BatchFrame* _cilk_frame = USE_SHARED(batch_frame);

  CILK2C_START_THREAD_FAST();

  work_array = _cilk_frame->args->work_array;

  CILK2C_BEFORE_SPAWN_FAST();
  CILK2C_PUSH_FRAME(_cilk_frame);

  //  op(ws, ds, (void*)work_array, num_ops, NULL);

  CILK2C_XPOP_FRAME_NORESULT(_cilk_frame, /* return nothing*/);
  CILK2C_AFTER_SPAWN_FAST();

  _cilk_frame->header.entry=2;
  CILK2C_AT_SYNC_FAST();

  --ws->current_cache->tail;
  CILK_WMB();

  Cilk_remove_and_keep_closure_and_frame(_cilk_ws, &_cilk_frame->header,
                                         _cilk_ws->self,
																				 USE_PARAMETER(ds_deques));

  // @todo @feature It would theoretically be helpful to spawn this
  // termination to do it in parallel. But in practice this is
  // unlikely to be useful until we have 128+ cores.
  Cilk_terminate_batch(_cilk_ws);
}

static void invoke_batch_slow(CilkWorkerState *const _cilk_ws,
                              BatchFrame *_cilk_frame)
{
  //	Cilk_enter_state(_cilk_ws, STATE_BATCH_INVOKE);
  InternalBatchOperation op;
	void* ds;
  void* work_array;
  unsigned num_ops = 0;
  CilkWorkerState *const ws = _cilk_ws; /*for the USE_SHARED macro at
																					the end of the func.*/

  CILK2C_START_THREAD_SLOW();
  switch (_cilk_frame->header.entry) {
  case 1:
    goto _sync1;
  case 2:
    goto _sync2;
  }

  op = _cilk_frame->args->op;
  ds = _cilk_frame->args->ds;
  work_array = _cilk_frame->args->work_array;
  num_ops = _cilk_frame->args->num_ops;

  _cilk_frame->header.entry=1;
  CILK2C_BEFORE_SPAWN_SLOW();
  CILK2C_PUSH_FRAME(_cilk_frame);

  //	Cilk_exit_state(ws, STATE_BATCH_INVOKE);
	op(_cilk_ws, ds, work_array, num_ops, NULL);

  CILK2C_XPOP_FRAME_NORESULT(_cilk_frame,/* return nothing */);
  CILK2C_AFTER_SPAWN_SLOW();
  //	Cilk_enter_state(ws, STATE_BATCH_INVOKE);

  if (0) {
  _sync1:
    ;
  }
  CILK2C_AT_THREAD_BOUNDARY_SLOW();

  CILK2C_BEFORE_SYNC_SLOW();
  _cilk_frame->header.entry=2;

  if (CILK2C_SYNC) {
    //		Cilk_exit_state(ws, STATE_BATCH_INVOKE);
    return;
  _sync2:
    ;
  }
  CILK2C_AFTER_SYNC_SLOW();
  CILK2C_AT_THREAD_BOUNDARY_SLOW();

  CILK_WMB();

  Cilk_remove_and_keep_closure_and_frame(_cilk_ws, &_cilk_frame->header,
																				 _cilk_ws->self,
																				 USE_PARAMETER(ds_deques));

  Cilk_terminate_batch(_cilk_ws);

  //Cilk_exit_state(ws, STATE_BATCH_INVOKE);
  return;
}

void reset_batch_closure(CilkContext *const context)
{
  Closure* t = USE_PARAMETER1(invoke_batch);
  BatchFrame* f = USE_SHARED1(batch_frame);

  //  t->join_counter = 0;
  f->header.join_counter = 0;
  t->status = CLOSURE_READY;
  f->header.entry = 0;
  WHEN_CILK_DEBUG(f->header.magic = CILK_STACKFRAME_MAGIC);
  WHEN_CILK_DEBUG(t->owner_ready_deque = NOBODY);
}

void Batcher_init(CilkContext *const context)
{
  Closure *t;
	BatchFrame *f;
  int i;
  int batch_limit;

  batch_limit = USE_PARAMETER1(active_size) * USE_PARAMETER1(batchlimit);
  //  if (!batch_limit) cnt_limit = 1;

  USE_SHARED1(pending_batch).array =
    Cilk_malloc_fixed(batch_limit * sizeof(BatchRecord));


  USE_SHARED1(pending_batch).size =
    batch_limit * USE_PARAMETER1(batchvals);

  for (i = 0; i < batch_limit; i++) {
    USE_SHARED1(pending_batch).array[i].status = DS_DONE;
  }

	USE_SHARED1(batch_lock) = 0;

	// @todo This may not actually be sizeof(int)! It could actually change,
	// but for now it's okay to specialize.
	USE_SHARED1(batch_work_array) =
    Cilk_malloc_fixed(batch_limit
                      * sizeof(int)
                      * USE_PARAMETER1(batchvals));


  /* create a frame for invoke_batch */
	t = Cilk_Closure_create_malloc(context, NULL);
  t->parent = (Closure *) NULL;

  f = Cilk_malloc(sizeof(BatchFrame));
  f->header.sig = USE_SHARED1(invoke_batch_sig);
  f->args = Cilk_malloc(sizeof(BatchArgs));

  t->frame = &f->header;

  /* Initialize the signature of a batch operation */
  memset( USE_SHARED1(invoke_batch_sig), 0 , 3*sizeof(CilkProcInfo) );
  USE_SHARED1(invoke_batch_sig)[0].size = 0;//sizeof(int);
  USE_SHARED1(invoke_batch_sig)[0].index = sizeof(BatchFrame);
  USE_SHARED1(invoke_batch_sig)[0].inlet = invoke_batch_slow;
  USE_SHARED1(invoke_batch_sig)[1].size = 0;//sizeof(int);
  USE_SHARED1(invoke_batch_sig)[1].inlet = NULL;//invoke_batch_catch_inlet;
  USE_SHARED1(invoke_batch_sig)[1].argsize = 0;//sizeof(BatchArgs);

  USE_PARAMETER1(invoke_batch) = t;
  USE_SHARED1(batch_frame) = f;
  reset_batch_closure(context);
}

void Batcher_cleanup(CilkContext *const context)
{
  Cilk_free(USE_SHARED1(batch_frame)->args);
  Cilk_free(USE_SHARED1(batch_frame));
  Cilk_free(USE_PARAMETER1(invoke_batch));
}

#endif
