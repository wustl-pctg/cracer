#include <stdio.h>
#include <stdlib.h>
#include <internal/abi.h>
#include "stattool.h"

static size_t g_num_reads = 0;
static size_t g_num_writes = 0;
static size_t g_num_detaches = 0;
__thread static int t_checking_disabled = 1;
#define INSERTS_PER_DETACH 6

static size_t get_num_inserts()
{
  __sync_synchronize();
  return g_num_detaches * INSERTS_PER_DETACH;
}

static size_t get_num_queries()
{
  __sync_synchronize();
  return g_num_reads + g_num_writes;
}

static inline void disable_checking() { t_checking_disabled++; }
static inline void enable_checking() { t_checking_disabled--; }
static inline bool should_check() { return t_checking_disabled == 0;  }


void __stattool_print_info()
{
  printf("Inserts: %zu\nQueries: %zu\n", get_num_inserts(), get_num_queries());
}

extern "C" void cilk_enter_begin() { disable_checking(); }
extern "C" void cilk_enter_end(__cilkrts_stack_frame *sf, void *rsp)
{
  enable_checking(); 
}

extern "C" void cilk_enter_helper_begin(__cilkrts_stack_frame* sf, 
                                        void* this_fn, void* rip)
{
  disable_checking(); 
}

extern "C" void cilk_spawn_prepare() { disable_checking(); }
extern "C" void cilk_spawn_or_continue(int in_continuation) { enable_checking(); }
//extern "C" void cilk_continue(__cilkrts_stack_frame* sf, char* new_sp) { enable_checking(); }

extern "C" void
cilk_detach_begin(__cilkrts_stack_frame* sf, void* this_fn, void* rip)
{
  disable_checking();
  __sync_fetch_and_add(&g_num_detaches, 1);
}

extern "C" void cilk_detach_end() { enable_checking(); }
extern "C" void cilk_sync_begin(__cilkrts_stack_frame* sf) { disable_checking(); }
extern "C" void cilk_sync_end() { enable_checking(); }
extern "C" void cilk_leave_begin(__cilkrts_stack_frame* sf) { disable_checking(); }
extern "C" void cilk_leave_end() { enable_checking(); }

static inline void
tsan_read(void *addr, size_t mem_size, void *rip)
{ 
  if (should_check())
    __sync_fetch_and_add(&g_num_reads, 1);
}

static inline void
tsan_write(void *addr, size_t mem_size, void *rip)
{
  if (should_check())
    __sync_fetch_and_add(&g_num_writes, 1);
}

extern "C" void 
__tsan_read1(void *addr) { tsan_read(addr, 1, __builtin_return_address(0)); }
extern "C" void 
__tsan_read2(void *addr) { tsan_read(addr, 2, __builtin_return_address(0)); }
extern "C" void 
__tsan_read4(void *addr) { tsan_read(addr, 4, __builtin_return_address(0)); }
extern "C" void 
__tsan_read8(void *addr) { tsan_read(addr, 8, __builtin_return_address(0)); }
extern "C" void 
__tsan_read16(void *addr) { tsan_read(addr, 16, __builtin_return_address(0)); }
extern "C" void 
__tsan_write1(void *addr) { tsan_write(addr, 1, __builtin_return_address(0)); }
extern "C" void 
__tsan_write2(void *addr) { tsan_write(addr, 2, __builtin_return_address(0)); }
extern "C" void 
__tsan_write4(void *addr) { tsan_write(addr, 4, __builtin_return_address(0)); }
extern "C" void 
__tsan_write8(void *addr) { tsan_write(addr, 8, __builtin_return_address(0)); }
extern "C" void 
__tsan_write16(void *addr) { tsan_write(addr, 16, __builtin_return_address(0)); }

extern "C" void __tsan_func_entry(void *pc){ }
extern "C" void __tsan_func_exit() { }
extern "C" void __tsan_init() { enable_checking(); }
extern "C" void __tsan_vptr_read(void **vptr_p) {}
extern "C" void __tsan_vptr_update(void **vptr_p, void *new_val) {}
