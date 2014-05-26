struct _cilk_fib_frame
{
  CilkStackFrame header;
  struct{int n;} scope0;
  struct{int x; int y;} scope1;
};

struct _cilk_fib_args
{
  int _cilk_proc_result;
  int n;
};

static void _cilk_fib_slow(CilkWorkerState*const _cilk_ws,
                           struct _cilk_fib_frame*_cilk_frame);

static CilkProcInfo _cilk_fib_sig[]={
  {
    sizeof(int),
    sizeof(struct _cilk_fib_frame),
    _cilk_fib_slow,0,0
  },
  {
    sizeof(int),
    ((size_t) ((char *)&((struct _cilk_fib_frame *) 0)->scope1.x -
               (char *)((struct _cilk_fib_frame *) 0))),0,0,0
  },
  {
    sizeof(int),
    ((size_t) ((char *)&((struct _cilk_fib_frame *) 0)->scope1.y -
               (char *)((struct _cilk_fib_frame *) 0))),0,0,0
  },
  {0,0,0,0,0}
};

int fib (CilkWorkerState *const _cilk_ws, int n)
{
  struct _cilk_fib_frame*_cilk_frame;
  {
    _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws,
                                         sizeof(struct _cilk_fib_frame),
                                         _cilk_fib_sig);
  };
  {
    Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header));
    Cilk_cilk2c_event_new_thread_maybe(_cilk_ws);
  };
  {
    if (n < 2) {
      int _cilk_temp0=(n);
      {
        Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header));
        Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header),
                                        sizeof(*_cilk_frame));
      };
      return _cilk_temp0;
    } else {
      int x;
      int y;
      {
        _cilk_frame->header.entry=1;
        _cilk_frame->scope0.n=n;
        Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));
        {
          Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header));
        };
        x = fib(_cilk_ws,n-1);
        {
          int __tmp;
          {
            if (Cilk_cilk2c_pop_check(_cilk_ws)) {
              __tmp = x;
              if (Cilk_exception_handler(_cilk_ws, &__tmp, sizeof(__tmp))) {
                Cilk_cilk2c_pop(_cilk_ws);
                return 0;
              }
            }
          };
        }
        {
          Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));
          Cilk_cilk2c_event_new_thread_maybe(_cilk_ws);
        };
      }
      {
        _cilk_frame->header.entry=2;
        _cilk_frame->scope1.x=x;
        Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));
        {
          Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header));
        };
        y = fib(_cilk_ws,n-2);
        {
          int __tmp;
          {
            if (Cilk_cilk2c_pop_check(_cilk_ws)) {
              __tmp = y;
              if (Cilk_exception_handler(_cilk_ws, &__tmp, sizeof(__tmp))) {
                Cilk_cilk2c_pop(_cilk_ws);
                return 0;
              }
            }
          };
        }
        {
          Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));
          Cilk_cilk2c_event_new_thread_maybe(_cilk_ws);
        };
      }
      {
        Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header));
        Cilk_cilk2c_event_new_thread_maybe(_cilk_ws);
      };
      {
        int _cilk_temp1=(x+y);
        {
          Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header));
          Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header),
                                          sizeof(*_cilk_frame));
        };
        return _cilk_temp1;
      }
    }
  }
}

static void _cilk_fib_slow(CilkWorkerState*const _cilk_ws,
                           struct _cilk_fib_frame*_cilk_frame)
{
  int n;
  {
    Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header));
    Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header));
  };
  switch (_cilk_frame->header.entry)
    {
    case 1: goto _cilk_sync1;
    case 2: goto _cilk_sync2;
    case 3: goto _cilk_sync3;
    }
  n=_cilk_frame->scope0.n;
  {
    if (n < 2) {
      {
        int __tmp=(n);
        Cilk_set_result(_cilk_ws,&__tmp,sizeof((__tmp)));
      }
      {
        Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header));
        Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header),
                                        sizeof(*_cilk_frame));
      };
      return;
    } else {
      {
        int _cilk_temp2;
        _cilk_frame->header.entry=1;
        _cilk_frame->scope0.n=n;
        Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));
        {
          Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header));
        };
        _cilk_temp2=fib(_cilk_ws,n-1);
        {
          int __tmp;
          {
            if (Cilk_cilk2c_pop_check(_cilk_ws)) {
              __tmp = _cilk_temp2;
              if (Cilk_exception_handler(_cilk_ws, &__tmp, sizeof(__tmp))) {
                Cilk_cilk2c_pop(_cilk_ws);
                return;
              }
            }
          };
        }
        Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));
        _cilk_frame->scope1.x=_cilk_temp2;
        if (0) {
        _cilk_sync1:n=_cilk_frame->scope0.n;
        }
        {
          Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws,
                                                  &(_cilk_frame->header));
          Cilk_cilk2c_event_new_thread_maybe(_cilk_ws);
        };
      }
      {
        int _cilk_temp3;
        _cilk_frame->header.entry=2;
        Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));
        {
          Cilk_cilk2c_push_frame(_cilk_ws, &(_cilk_frame->header));
        };
        _cilk_temp3=fib(_cilk_ws,n-2);
        {
          int __tmp;
          {
            if (Cilk_cilk2c_pop_check(_cilk_ws)) {
              __tmp = _cilk_temp3;
              if (Cilk_exception_handler(_cilk_ws, &__tmp, sizeof(__tmp))) {
                Cilk_cilk2c_pop(_cilk_ws);
                return ;
              }
            }
          };
        }
        Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header));
        _cilk_frame->scope1.y=_cilk_temp3;
        if (0) {
        _cilk_sync2:;
        }
        {
          Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header));
          Cilk_cilk2c_event_new_thread_maybe(_cilk_ws);
        };
      }
      {
        Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));
        _cilk_frame->header.entry=3;
        if (Cilk_sync(_cilk_ws)) {
          return;
        _cilk_sync3:;
        }
        Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header));
        {
          Cilk_cilk2c_at_thread_boundary_slow_cp( _cilk_ws, &(_cilk_frame->header));
          Cilk_cilk2c_event_new_thread_maybe(_cilk_ws);
        };
      }
      {
        {
          int __tmp=(_cilk_frame->scope1.x+_cilk_frame->scope1.y);
          Cilk_set_result(_cilk_ws,&__tmp,sizeof((__tmp)));
        }
        {
          Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header));
          Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header),
                                          sizeof(*_cilk_frame));
        };
        return;
      }
    }
  }
}
