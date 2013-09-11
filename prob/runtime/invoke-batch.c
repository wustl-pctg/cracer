#include <stdio.h> // ***
#include <string.h>
#include <cilk.h>
//#include <cilk-internal.h>
#include <cilk-cilk2c-pre.h>
#include <cilk-cilk2c.h>

/************************************************************
 * Batcher-related routines
 ************************************************************/
void Cilk_terminate_batch(CilkWorkerState *const ws)
{
	int i, index;
	Cilk_enter_state(ws, STATE_BATCH_TERMINATE);
	Batch *current = &USE_SHARED(pending_batch);
	void* results = USE_SHARED(batch_work_array);
	size_t dataSize = current->dataSize;

	for (i = 0; i < USE_PARAMETER(active_size); i++) {
		if (current->array[i].status == DS_IN_PROGRESS) {
			index = current->array[i].packedIndex;
			if (current->array[i].result) {
				memcpy(current->array[i].result, &results[index], dataSize);
			}
			current->array[i].status = DS_DONE;
		}
	}
	i = USE_SHARED(current_batch_id);
	USE_SHARED(current_batch_id)++; // signal end of this batch
	Cilk_exit_state(ws, STATE_BATCH_TERMINATE);
}

struct invoke_batch_catch_inlet_args {int res;};

void invoke_batch_catch_inlet(CilkWorkerState *const _cilk_ws,
															invoke_batch_frame *_cilk_frame,
															BatchArgs *inletargs,
															void* UNUSED(inletresult))
{
  memcpy(_cilk_frame->args, inletargs, _cilk_frame->arg_size);
  CILK2C_ABORT_STANDALONE();
}

static void invoke_batch_slow(CilkWorkerState *const _cilk_ws,
															invoke_batch_frame *_cilk_frame)
{
	void *dataStruct;
	void *data;
	size_t numElements;
	void *result;
  CilkWorkerState *const ws = _cilk_ws; /*for the USE_SHARED macro at the end of the func.*/

  CILK2C_START_THREAD_SLOW();
  switch (_cilk_frame->header.entry) {
  case 1:
    goto _sync1;
  case 2:
    goto _sync2;
  }

  _cilk_ws->cp_hack = 0;
  _cilk_ws->work_hack = 0;
  _cilk_ws->user_work = 0;
  _cilk_ws->user_critical_path = 0;
  WHEN_CILK_TIMING(_cilk_frame->header.cp = (Cilk_time) 0);
  WHEN_CILK_TIMING(_cilk_frame->header.work = (Cilk_time) 0);
  WHEN_CILK_TIMING(_cilk_frame->header.mycp = (Cilk_time) 0);
  WHEN_CILK_TIMING(_cilk_ws->last_cp_time = Cilk_get_time());

  dataStruct = _cilk_frame->args->dataStruct;
	data = _cilk_frame->args->data;
	numElements = _cilk_frame->args->numElements;
	result = _cilk_frame->args->result;

  _cilk_frame->header.receiver = (void *) &_cilk_frame->retval;
  _cilk_frame->header.entry=1;
  CILK2C_BEFORE_SPAWN_SLOW();
  CILK2C_PUSH_FRAME(_cilk_frame);

  _cilk_frame->batch_op(_cilk_ws, dataStruct, data, numElements, result);
  CILK2C_XPOP_FRAME_NORESULT(_cilk_frame,/* return nothing */);
  CILK2C_AFTER_SPAWN_SLOW();


  if (0) {
  _sync1:
    ;
  }
  CILK2C_AT_THREAD_BOUNDARY_SLOW();

  CILK2C_BEFORE_SYNC_SLOW();
  _cilk_frame->header.entry=2;
  if (CILK2C_SYNC) {
    return;
  _sync2:
    ;
  }
  CILK2C_AFTER_SYNC_SLOW();
  CILK2C_AT_THREAD_BOUNDARY_SLOW();

  WHEN_CILK_TIMING(USE_SHARED(critical_path) = _cilk_frame->header.mycp);
  WHEN_CILK_TIMING(USE_SHARED(total_work) = _cilk_frame->header.work);

  CILK_WMB();
	Cilk_terminate_batch(_cilk_ws);

  Cilk_remove_and_free_closure_and_frame(_cilk_ws, &_cilk_frame->header,
																				 _cilk_ws->self,
																				 USE_PARAMETER(ds_deques));
  return;

}

static void Batch_create(CilkWorkerState *const ws, InternalBatchOperation op,
												 size_t dataSize)
{
	int numJobs = 0, i;
	void *workArray = USE_SHARED(batch_work_array);
	Batch *pending = &USE_SHARED(pending_batch);
	USE_SHARED(batch_owner) = ws->self; // need this?***

	if (workArray == NULL) {
		workArray = Cilk_malloc(dataSize * USE_PARAMETER(active_size));
		USE_SHARED(batch_work_array) = workArray;
	} else if (pending->dataSize < dataSize) { // last time we used a different operation with different datasize
		Cilk_free(workArray);
		workArray = Cilk_malloc(dataSize * USE_PARAMETER(active_size));
		USE_SHARED(batch_work_array) = workArray;
	}

	for (i = 0; i < USE_PARAMETER(active_size); i++) {
		if (pending->array[i].status == DS_WAITING &&
				pending->array[i].operation == op) {
			memcpy(workArray + dataSize*numJobs,
						 pending->array[i].args, dataSize);
			pending->array[i].packedIndex = numJobs;
			pending->array[i].status = DS_IN_PROGRESS;
			if ((USE_PARAMETER(options->btest) & 1) == 1) {
				USE_SHARED(batch_workers_list)[numJobs] = i;
			}
			numJobs++;
		}
	}

	pending->dataSize = dataSize;
	pending->operation = op;
	pending->size = numJobs;
#if CILK_STATS
	USE_SHARED(batch_sizes)[numJobs-1]++;
#endif

}

/* Closure * Batcher_create_batch_closure(CilkWorkerState *const ws, */
/* 																			 InternalBatchOperation op, */
/* 																			 size_t dataSize, */
/* 																			 void *dataStruct) */
//Closure * Batcher_create_batch_closure(CilkContext *const context,
Closure * Batcher_create_batch_closure(CilkWorkerState *const ws,
																			 InternalBatchOperation op,
																			 BatchArgs *args)
{
	Closure *t;
	invoke_batch_frame *f;
	int arg_size;
	//	BatchArgs args;
	//	Batch *pending = &USE_SHARED(pending_batch);

	//	Cilk_enter_state(ws, STATE_BATCH_START);
	arg_size = sizeof(BatchArgs);
	//	Batch_create(ws, op, dataSize);

  /* create a frame for invoke_batch */
	t = Cilk_Closure_create_malloc(ws->context, NULL);
  t->parent = (Closure *) NULL;
  t->join_counter = 0;
	t->status = CLOSURE_READY;

  f = Cilk_malloc(sizeof(invoke_batch_frame));
	//	f = &USE_SHARED(batch_frame);
  f->header.entry = 0;
  f->header.sig = USE_SHARED(invoke_batch_sig);
  WHEN_CILK_DEBUG(f->header.magic = CILK_STACKFRAME_MAGIC);

	f->args = args;
	/* f->args = &USE_SHARED(batch_args); */
	/* f->args->dataStruct = dataStruct; */
	/* f->args->data = &USE_SHARED(batch_work_array); */
	/* f->args->numElements = pending->size; */
	/* f->args->result = f->args->data; */
  f->arg_size = arg_size;
  f->batch_op = op;

  t->frame = &f->header;

  /* Initialize the signature of a batch operation */
  memset( USE_SHARED(invoke_batch_sig), 0 , 3*sizeof(CilkProcInfo) );
  USE_SHARED(invoke_batch_sig)[0].size = sizeof(int);
  USE_SHARED(invoke_batch_sig)[0].index = sizeof(invoke_batch_frame);
  USE_SHARED(invoke_batch_sig)[0].inlet = invoke_batch_slow;
  USE_SHARED(invoke_batch_sig)[1].size = sizeof(int);
  USE_SHARED(invoke_batch_sig)[1].inlet = invoke_batch_catch_inlet;
  USE_SHARED(invoke_batch_sig)[1].argsize = arg_size;

	/* Cilk_exit_state(ws, STATE_BATCH_START); */

	return t;
}