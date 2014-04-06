#ifndef _CILK_INVOKE_BATCH_C
#define _CILK_INVOKE_BATCH_C

static inline set_batch_info(Batch* pending, InternalBatchOperation op,
                             size_t size, size_t data_size, void* ds)
{
  pending->operation = op;
  pending->size = size;
  pending->data_size = data_size;
  pending->data_structure = ds;
}

static inline int* compact(CilkWorkerState *const ws)
{
  unsigned int num_ops = 0;
  unsigned int register i;
  Batch *pending = &USE_SHARED(pending_batch);
  int *work_array = USE_SHARED(batch_work_array);

  InternalBatchOperation op = NULL;
  size_t data_size = 0;
  void* ds = NULL;

  for (i = 0; i < USE_PARAMETER(num_real_workers); i++) {

    // @todo @feature Should also check that the op matches, for multiple
    // batch ops.
    if (pending->array[i].status == DS_WAITING) {

      pending->array[i].status = DS_IN_PROGRESS;
      pending->array[i].packed_index = num_ops;

      if (op == NULL) {
        op = pending->array[i].operation;
        data_size = pending->array[i].size;
        ds = pending->array[i].data_structure;
      }

      // @todo @feature Use memcpy here (or other) to support any data type.
      work_array[num_ops] = *((int*)pending->array[i].args);

      num_ops++;
    }
  }

  CILK_ASSERT(ws, ds != NULL);
  CILK_ASSERT(ws, op != NULL);
  set_batch_info(pending, op, num_ops, data_size, ds);
  return work_array;
}

// @todo @feature Also copy back the results, if the result array is not NULL.
static inline void terminate_batch(CilkWorkerState *const ws)
{
  int i, index;
	Batch* current = &USE_SHARED(pending_batch);

  for (i = 0; i < USE_PARAMETER(num_real_workers); i++) {
    if (current->array[i].status == DS_IN_PROGRESS) {
			current->array[i].status = DS_DONE;
		}
	}
  /* USE_SHARED(current_batch_id)++; // signal end of this batch */
  /* USE_SHARED(batch_owner) = -1; */
  /* __sync_lock_release(&USE_SHARED(batch_lock)); */
}

struct invoke_ds_main_frame {
  CilkStackFrame header;
  int return_size;
  int retval;
};

// @todo @opt Slightly faster with a try_compact function instead of calling
// have_ds_work() and then compact().
static inline unsigned int have_ds_work(CilkContext* context)
{
  unsigned int register i;
  Batch* pending = &USE_SHARED1(pending_batch);
  unsigned int have_work = 0;

  for (i = 0; i < USE_PARAMETER1(num_real_workers); i++) {
    if (pending->array[i].status == DS_WAITING) {
      have_work = 1;
      break;
    }
  }
  return have_work;
}

static inline void wait_for_ds_work(CilkWorkerState *const ws)
{
  struct timespec temp;
  temp.tv_sec = 0;
  temp.tv_nsec = 1000L;

  while (!have_ds_work(ws->context) && !USE_SHARED(done))
    nanosleep(&temp, NULL);
    //    nanosleep(&USE_PARAMETER(sleeptime), NULL);
}

static void invoke_ds_main_slow(CilkWorkerState *const _cilk_ws,
                                struct invoke_ds_main_frame* _cilk_frame)
{
  CilkWorkerState *const ws = _cilk_ws;
  Batch *pending = &USE_SHARED(pending_batch);
  int* work_array;

  CILK2C_START_THREAD_SLOW();
  switch (_cilk_frame->header.entry) {
  case 1:
    goto _sync1;
  case 2:
    goto _sync2;
  }

  while (1)
  {
    wait_for_ds_work(_cilk_ws);
    if (USE_SHARED(done)) break;

    work_array = compact(_cilk_ws);

    // prepare for spawn...
    _cilk_frame->header.entry = 1;
    CILK2C_BEFORE_SPAWN_SLOW();
    CILK2C_PUSH_FRAME(_cilk_frame);

    // now call the batch operation...
    pending->operation(ws, pending->data_structure, work_array,
                       pending->size, NULL);

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
      //		Cilk_exit_state(ws, STATE_BATCH_INVOKE);
      return;
      _sync2:
      ;
    }
    CILK2C_AFTER_SYNC_SLOW();
    CILK2C_AT_THREAD_BOUNDARY_SLOW();

    CILK_WMB();

    terminate_batch(_cilk_ws);

  }

  // I may actually need to call
  // Cilk_remove_and_keep_closure_and_frame, depending on if workers
  // will steal this and becomes the ds_main worker.
  return;
}

Closure *Cilk_create_initial_ds_thread(CilkContext *const context)
/* void (*import_ds_main)(CilkWorkerState *const ws, void *args), */
/* void *args, */
/* int return_size) */
{
  Closure *t;
  struct invoke_main_frame *f;
  int i;

  USE_SHARED1(pending_batch).array =
    Cilk_malloc_fixed(USE_PARAMETER1(active_size) * sizeof(BatchRecord));
  USE_SHARED1(pending_batch).size = USE_PARAMETER1(active_size);
  // * USE_PARAMETER1(batchvals);

  for (i = 0; i < USE_PARAMETER1(active_size); ++i) {
    USE_SHARED1(pending_batch).array[i].status = DS_DONE;
  }

  // This may not actually be sizeof(int)! It could actually change,
	// but for now it's okay to specialize. ***
	USE_SHARED1(batch_work_array) =
    Cilk_malloc_fixed(USE_PARAMETER1(active_size)
                      * sizeof(int));
                      //                    * USE_PARAMETER1(batchvals));


  /* create a frame for invoke_cilk_main */
  t = Cilk_Closure_create_malloc(context, NULL);
  t->parent = (Closure *) NULL;
  t->join_counter = 0;
  t->status = CLOSURE_READY;

  f = Cilk_malloc(sizeof(struct invoke_ds_main_frame));
  f->header.entry = 0;
  f->header.sig = USE_SHARED1(invoke_ds_main_sig);
  WHEN_CILK_DEBUG(f->header.magic = CILK_STACKFRAME_MAGIC);

  /* f->args = args; */
  /* f->return_size = return_size; */
  /* f->import_main = import_ds_main; */

  t->frame = &f->header;

  /* Initialize the signature of Cilk_main */
  memset( USE_SHARED1(invoke_ds_main_sig), 0 , 3*sizeof(CilkProcInfo) );
  USE_SHARED1(invoke_ds_main_sig)[0].size = sizeof(int);
  USE_SHARED1(invoke_ds_main_sig)[0].index
    = sizeof(struct invoke_ds_main_frame);
  USE_SHARED1(invoke_ds_main_sig)[0].inlet = invoke_ds_main_slow;
  USE_SHARED1(invoke_ds_main_sig)[1].size = 0;//sizeof(int);
  USE_SHARED1(invoke_ds_main_sig)[1].inlet = NULL;//invoke_ds_main_catch_inlet;
  USE_SHARED1(invoke_ds_main_sig)[1].argsize = 0;//return_size;

  return t;
}




#endif
