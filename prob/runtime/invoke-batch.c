#ifndef _CILK_INVOKE_BATCH_C
#define _CILK_INVOKE_BATCH_C
#include <string.h>
#include <cilk.h>
#include <cilk-cilk2c-pre.h>
#include <cilk-cilk2c.h>

#include <stdio.h> // ***

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

static inline unsigned int compact(CilkWorkerState *const ws, Batch *pending,
                                   helper* work_array, BatchRecord *record)
                                   //        InternalBatchOperation op)
{
  unsigned int num_ops = 0;
  unsigned int register i;

  for (i = 0; i < USE_PARAMETER(active_size); i++) {

    // Should also check that the op matches, for multiple batch operations***
    if (pending->array[i].status == DS_WAITING) {
      pending->array[i].status = DS_IN_PROGRESS;
      //			pending->array[i].packedIndex = num_ops;

      work_array[num_ops] = *(helper*)pending->array[i].args;
      num_ops++;
    }
  }

  return num_ops;
}

static inline void Cilk_terminate_batch(CilkWorkerState *const ws)
{
  /* printf("Worker %i terminating global: %i, local: %i.\n", ws->self, */
  /*        USE_SHARED(current_batch_id), ws->batch_id); */
  if (USE_SHARED(current_batch_id) > ws->batch_id) {
    printf("Cilk_terminate_batch executed too many times!\n");
  }
  int i, index;
	//	Cilk_enter_state(ws, STATE_BATCH_TERMINATE);
	Batch current = USE_SHARED(pending_batch);
  //	void* results = USE_SHARED(batch_work_array);
  //	size_t dataSize = current->dataSize;

	for (i = 0; i < USE_PARAMETER(active_size); i++) {
    if (current.array[i].status == DS_IN_PROGRESS) {
			//			index = current->array[i].packedIndex;
			/* if (current->array[i].result) { */
			/* 	memcpy(current->array[i].result, &results[index], dataSize); */
			/* } */
      //      printf("Spot %i is done for batch %i.\n", i, ws->batch_id);
			current.array[i].status = DS_DONE;
		}
	}

  /* USE_SHARED(current_batch_id)++; // signal end of this batch */
  /* USE_SHARED(batch_owner) = -1; */
  /* USE_SHARED(batch_lock) = 0; */
	//	Cilk_exit_state(ws, STATE_BATCH_TERMINATE);
}

/* struct invoke_batch_catch_inlet_args {int res;}; */

/* void invoke_batch_catch_inlet(CilkWorkerState *const _cilk_ws, */
/* 															BatchFrame *_cilk_frame, */
/* 															BatchArgs *inletargs, */
/* 															void* UNUSED(inletresult)) */
/* { */
/*   memcpy(_cilk_frame->args, inletargs, _cilk_frame->arg_size); */
/*   CILK2C_ABORT_STANDALONE(); */
/* } */

/* static void invoke_batch(CilkWorkerState* const _cilk_ws, InternalBatchOperation op, */
/*                   void* ds, void* work_array, int num_ops, void* result_array) */
static void invoke_batch(CilkWorkerState* const _cilk_ws,
                         InternalBatchOperation op,
                         Batch* pending)
{
  CilkWorkerState* const ws = _cilk_ws;
  unsigned int num_ops = 0;
  helper* work_array = USE_SHARED(batch_work_array);

  /* BatchFrame* _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(BatchFrame), */
  /*                                                  USE_SHARED(invoke_batch_sig)); */
  BatchFrame* _cilk_frame = USE_SHARED(batch_frame);
  volatile CilkStackFrame **t = ws->current_cache->tail;
  CILK_COMPLAIN((CilkStackFrame **) t < ws->current_cache->stack + ws->stackdepth,
								(ws->context, ws, USE_PARAMETER(stack_overflow_msg)));

  *t = (CilkStackFrame *) _cilk_frame;
  Cilk_membar_StoreStore();
  ws->current_cache->tail = t + 1;


  CILK2C_START_THREAD_FAST();

  // We should do the compaction here. This will allow us to spawn and
  // do the compaction in parallel, although this will likely only
  // help on a massively parallel machine, e.g. 128+ threads.
  _cilk_frame->header.entry = 1;
  //  _cilk_frame->header.receiver = (void *) &_cilk_frame->retval;
  // Set args here
  //  _cilk_frame->scope0.n = n;

  CILK2C_BEFORE_SPAWN_FAST();
  CILK2C_PUSH_FRAME(_cilk_frame);

  num_ops = compact(ws, pending, work_array, NULL);

  //	_cilk_frame->batch_op(_cilk_ws, dataStruct, data, numElements, result);
  op(ws, ds, (void*)work_array, num_ops, NULL);

  CILK2C_XPOP_FRAME_NORESULT(_cilk_frame, /* return nothing*/);
  CILK2C_AFTER_SPAWN_FAST();

  _cilk_frame->header.entry=2;
  CILK2C_AT_SYNC_FAST();

  // We can also spawn here, to do in parallel, optionally.

  // Don't want this, because it frees the frame.
  //  CILK2C_BEFORE_RETURN_FAST();
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
  //  printf("Invoking batch %i, worker %i.\n", _cilk_ws->batch_id, _cilk_ws->self);
  //	Cilk_enter_state(_cilk_ws, STATE_BATCH_INVOKE);
	void *dataStruct;
	void *data;
	size_t numElements;
	void *result;
  CilkWorkerState *const ws = _cilk_ws; /*for the USE_SHARED macro at
																					the end of the func.*/

  CILK2C_START_THREAD_SLOW();
  switch (_cilk_frame->header.entry) {
  case 1:
    goto _sync1;
  case 2:
    goto _sync2;
  }

  dataStruct = _cilk_frame->args->dataStruct;
	data = _cilk_frame->args->data;
	numElements = _cilk_frame->args->numElements;
	result = _cilk_frame->args->result;

  _cilk_frame->header.receiver = (void *) &_cilk_frame->retval;
  _cilk_frame->header.entry=1;
  CILK2C_BEFORE_SPAWN_SLOW();
  CILK2C_PUSH_FRAME(_cilk_frame);

  //	Cilk_exit_state(ws, STATE_BATCH_INVOKE);
	_cilk_frame->batch_op(_cilk_ws, dataStruct, data, numElements, result);


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
  //	Cilk_exit_state(ws, STATE_BATCH_INVOKE);
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
	int arg_size;

  /* create a frame for invoke_batch */
	t = Cilk_Closure_create_malloc(context, NULL);
  t->parent = (Closure *) NULL;

  f = Cilk_malloc(sizeof(BatchFrame));
  f->header.sig = USE_SHARED1(invoke_batch_sig);
  f->args = Cilk_malloc(sizeof(BatchArgs));

  t->frame = &f->header;

  /* Initialize the signature of a batch operation */
  memset( USE_SHARED1(invoke_batch_sig), 0 , 3*sizeof(CilkProcInfo) );
  USE_SHARED1(invoke_batch_sig)[0].size = sizeof(int);
  USE_SHARED1(invoke_batch_sig)[0].index = sizeof(BatchFrame);
  USE_SHARED1(invoke_batch_sig)[0].inlet = invoke_batch_slow;
  USE_SHARED1(invoke_batch_sig)[1].size = sizeof(int);
  USE_SHARED1(invoke_batch_sig)[1].inlet = NULL;//invoke_batch_catch_inlet;
  USE_SHARED1(invoke_batch_sig)[1].argsize = sizeof(BatchArgs);//arg_size;

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

/* Closure * Batcher_create_batch_closure(CilkWorkerState *const ws, */
/* 																			 InternalBatchOperation op, */
/* 																			 BatchArgs *args) */
/* { */
/* 	Closure *t; */
/*   BatchFrame *f; */
/* 	int arg_size; */
/* 	//	BatchArgs args; */
/* 	//	Batch *pending = &USE_SHARED(pending_batch); */

/* 	//	Cilk_enter_state(ws, STATE_BATCH_START); */
/* 	arg_size = sizeof(BatchArgs); */
/* 	//	Batch_create(ws, op, dataSize); */

/*   /\* create a frame for invoke_batch *\/ */
/* 	t = Cilk_Closure_create_malloc(ws->context, NULL); */
/*   // *** Calls Closure_init, which calls mutex_init and sets up inlets */

/* 	/\* t = &USE_SHARED(invoke_batch_closure); *\/ */
/*   t->parent = (Closure *) NULL; */
/*   t->join_counter = 0; */
/* 	t->status = CLOSURE_READY; */


/*   f = Cilk_malloc(sizeof(invoke_batch_frame)); */
/* 	//	f = &USE_SHARED(batch_frame); */
/*   f->header.entry = 0; */
/*   f->header.sig = USE_SHARED(invoke_batch_sig); */
/*   WHEN_CILK_DEBUG(f->header.magic = CILK_STACKFRAME_MAGIC); */

/* 	f->args = args; */
/* 	/\* f->args = &USE_SHARED(batch_args); *\/ */
/* 	/\* f->args->dataStruct = ds; *\/ */
/* 	/\* f->args->data = pending->data; *\/ */
/* 	/\* f->args->result = result; *\/ */
/* 	/\* f->args->numElements = pending->size; *\/ */
/*   f->arg_size = arg_size; */
/*   f->batch_op = op; */

/*   t->frame = &f->header; */

/*   /\* Initialize the signature of a batch operation *\/ */
/*   memset( USE_SHARED(invoke_batch_sig), 0 , 3*sizeof(CilkProcInfo) ); */
/*   USE_SHARED(invoke_batch_sig)[0].size = sizeof(int); */
/*   USE_SHARED(invoke_batch_sig)[0].index = sizeof(invoke_batch_frame); */
/*   USE_SHARED(invoke_batch_sig)[0].inlet = invoke_batch_slow; */
/*   USE_SHARED(invoke_batch_sig)[1].size = sizeof(int); */
/*   USE_SHARED(invoke_batch_sig)[1].inlet = invoke_batch_catch_inlet; */
/*   USE_SHARED(invoke_batch_sig)[1].argsize = arg_size; */

/* 	/\* Cilk_exit_state(ws, STATE_BATCH_START); *\/ */

/* 	return t; */
/* } */
#endif
