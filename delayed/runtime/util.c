struct _cilk_delayed_batchify_frame
{
  CilkStackFrame header;
  struct
  {
    InternalBatchOperation operation;
    void*data;
    size_t data_size;
    void*ds;
    void*result;
  } scope0;
};

struct _cilk_delayed_batchify_args
{
  InternalBatchOperation operation;
  void*data;
  size_t data_size;
  void*ds;
  void*result;
};

static void _cilk_delayed_batchify_slow(CilkWorkerState*const _cilk_ws,
                                        struct _cilk_delayed_batchify_frame*_cilk_frame);
static CilkProcInfo _cilk_delayed_batchify_sig[]=
  {{0,sizeof(struct _cilk_delayed_batchify_frame),
    _cilk_delayed_batchify_slow,0,0}};

#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_FAST_PROCEDURE
# 4
void delayed_batchify(CilkWorkerState*const _cilk_ws,
                      InternalBatchOperation operation, void*data,
                      size_t data_size, void*ds,  void*result)
{
  /* struct _cilk_delayed_batchify_frame*_cilk_frame; */
  /* CILK2C_INIT_FRAME(_cilk_frame,sizeof(struct _cilk_delayed_batchify_frame), */
  /*                   _cilk_delayed_batchify_sig); */
  /* CILK2C_START_THREAD_FAST(); */
  {
    internal_delayed_batchify(_cilk_ws, operation, data, data_size, ds, result);
    {
      //      CILK2C_BEFORE_RETURN_FAST();
      return;
    }
  }
}

#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_SLOW_PROCEDURE
# 4
static void _cilk_delayed_batchify_slow(CilkWorkerState*const _cilk_ws,
                                        struct _cilk_delayed_batchify_frame*_cilk_frame)
{
  InternalBatchOperation operation;
  void*data;
  size_t data_size;
  void*ds;
  void*result;
  /* CILK2C_START_THREAD_SLOW(); */
  /* switch (_cilk_frame->header.entry) {} */
  /* operation=_cilk_frame->scope0.operation; */
  /* data=_cilk_frame->scope0.data; */
  /* data_size=_cilk_frame->scope0.data_size; */
  /* ds=_cilk_frame->scope0.ds; */
  /* result=_cilk_frame->scope0.result; */
  {
    internal_delayed_batchify(_cilk_ws, operation, data, data_size, ds, result);
    {
      /* CILK2C_SET_NORESULT(); */
      /* CILK2C_BEFORE_RETURN_SLOW(); */
      return;
    }
  }
}
#undef CILK_WHERE_AM_I
#define CILK_WHERE_AM_I IN_C_CODE
# 4
