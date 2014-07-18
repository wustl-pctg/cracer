#ifndef _CILK_INVOKE_BATCH_C
#define _CILK_INVOKE_BATCH_C
#include <string.h>
#include <cilk.h>
#include <cilk-cilk2c-pre.h>
#include <cilk-cilk2c.h>

/***********************************************************!
 * Pre-declared functions from sched.c for OM_DS functions *
 ***********************************************************/
void OM_DS_before_spawn(CilkWorkerState *const ws, CilkStackFrame *frame, const int FAST_NOT_SLOW);

void OM_DS_sync_slow(CilkWorkerState *const ws, CilkStackFrame *frame);

void OM_DS_sync_fast(CilkWorkerState *const ws, CilkStackFrame *frame);

void OM_DS_new_thread_start(CilkWorkerState *const ws, CilkStackFrame *frame);


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

// We assume that work_array is large enough.
static inline unsigned int compact(CilkWorkerState *const ws, Batch *pending,
                                   void* work_array, BatchRecord *record)
{
	CILK_ASSERT(ws, ws->batch_id == USE_SHARED(current_batch_id));

	Cilk_enter_state(ws, STATE_BATCH_COMPACTION);
	unsigned int register i,j;
	unsigned int num_ops = 0;
	unsigned int num_spots = USE_PARAMETER(batchvals);

	for (i = 0; i < USE_PARAMETER(active_size); i++) {

		// Should also check that the op matches, for multiple batch operations***
		if (DS_WAITING == pending->array[i].status &&
			record->operation == pending->array[i].operation) {

			pending->array[i].status = DS_IN_PROGRESS;
			pending->array[i].packed_index = num_ops;

			memcpy(work_array + (num_ops * pending->data_size),
				   pending->array[i].args,
				   pending->data_size * num_spots);

			num_ops += num_spots;
		}
	}

	pending->operation = record->operation;
	pending->data_size = record->size;
	pending->size = num_ops;

	Cilk_exit_state(ws, STATE_BATCH_COMPACTION);
#if CILK_STATS
	USE_SHARED(num_batches)++;
	USE_SHARED(batch_sizes)[(num_ops / USE_PARAMETER(batchvals)) - 1]++;
#endif
	return num_ops;
}

static inline void Cilk_terminate_batch(CilkWorkerState *const ws)
{
	CILK_ASSERT(ws, USE_SHARED(current_batch_id) ==
				USE_SHARED(pending_batch).batch_no);
	CILK_ASSERT(ws, ws->batch_id == USE_SHARED(current_batch_id));
	Cilk_enter_state(ws, STATE_BATCH_TERMINATE);

	int i, index;
	Batch* current = &USE_SHARED(pending_batch);
	//	void* results = USE_SHARED(batch_work_array);
	//	size_t dataSize = current->dataSize;

	for (i = 0; i < USE_PARAMETER(active_size); i++) {
		if (current->array[i].status == DS_IN_PROGRESS) {
			//			index = current->array[i].packedIndex;
			/* if (current->array[i].result) { */
			/* 	memcpy(current->array[i].result, &results[index], dataSize); */
			/* } */
			current->array[i].status = DS_DONE;
		}
	}

	USE_SHARED(current_batch_id)++; // signal end of this batch
	USE_SHARED(batch_owner) = -1;
	__sync_lock_release(&USE_SHARED(batch_lock));
	Cilk_exit_state(ws, STATE_BATCH_TERMINATE);
}

// @todo @refactor I don't think this actually helps anything. We
// still have to put an initial closure on the deque
// (setup_for_execution).
// Whether we put invoke_batch_slow on initially or the actual batch
// function doesn't particularly matter -- someone stealing
// invoke_batc will see that it is executing and just suspend and
// steal again.
// NB: I *think* this is what happens. I should check empirically just
// to be safe.
static void invoke_batch(CilkWorkerState* const _cilk_ws, void* dataStruct,
                         InternalBatchOperation op,
                         unsigned int num)
{
	CilkWorkerState* const ws = _cilk_ws;
	CILK_ASSERT(ws, ws->batch_id == USE_SHARED(current_batch_id));
	unsigned int num_ops = num;
	int* work_array;
	void* ds = dataStruct;

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

	op(ws, ds, (void*)work_array, num_ops, NULL);

	CILK2C_XPOP_FRAME_NORESULT(_cilk_frame, /* return nothing*/);
	CILK2C_AFTER_SPAWN_FAST();

	_cilk_frame->header.entry=2;
	CILK2C_AT_SYNC_FAST();

	// We can also spawn here, to do in parallel, optionally.

	--ws->current_cache->tail;
	CILK_WMB();

	Cilk_remove_and_keep_closure_and_frame(_cilk_ws, &_cilk_frame->header,
										   _cilk_ws->self,
										   USE_PARAMETER(ds_deques));
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

	//  CILK_ASSERT(ws, ws->batch_id == USE_SHARED(current_batch_id));

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

	t->join_counter = 0;
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

	USE_SHARED1(pending_batch).array =
		Cilk_malloc_fixed(USE_PARAMETER1(active_size)
						  * sizeof(BatchRecord));

	USE_SHARED1(pending_batch).size =
		USE_PARAMETER1(active_size) * USE_PARAMETER1(batchvals);

	USE_SHARED1(pending_batch).data_size = sizeof(int);

	for (i = 0; i < USE_PARAMETER1(active_size); i++) {
		USE_SHARED1(pending_batch).array[i].status = DS_DONE;
	}

	USE_SHARED1(batch_lock) = 0;

	// Note that the pending_batch data size may change, but in that
	// case the array will be resized in Cilk_batchify by the batch owner.
	USE_SHARED1(batch_work_array) =
		Cilk_malloc(USE_PARAMETER1(active_size)
					* USE_SHARED1(pending_batch).data_size
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
	// Simulation of Closure_destroy_malloc without access to a worker
	// state.
	Closure *t = USE_PARAMETER1(invoke_batch);

	//  WHEN_CILK_DEBUG(CILK_ASSERT(NULL, t->magic == CILK_CLOSURE_MAGIC));
	WHEN_CILK_DEBUG(t->magic = ~CILK_CLOSURE_MAGIC);
	//	WHEN_CILK_DEBUG(CILK_ASSERT1(NULL, t->malloced));
	//  Closure_clean(context, t);
	//et  Cilk_free(t);


	Cilk_free(USE_SHARED1(batch_work_array));
	Cilk_free(USE_SHARED1(batch_frame)->args);
	Cilk_free(USE_SHARED1(batch_frame));
	Cilk_free(USE_PARAMETER1(invoke_batch));
}
#endif
