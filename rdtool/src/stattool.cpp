#include <stdlib.h>
#include "stattool.h"

static size_t num_reads = 0;
static size_t num_writes = 0;
static size_t num_detaches = 0;
#define INSERTS_PER_DETACH 6

size_t get_num_inserts() { return num_detaches * INSERTS_PER_DETACH; }
size_t get_num_queries() { return num_reads + num_writes; }

extern "C" void
do_detach_begin(__cilkrts_stack_frame* sf, void* this_fn, void* rip)
{
	num_detaches++;
}

static inline void
tsan_read(void *addr, size_t mem_size, void *rip) { num_reads++; }

static inline void
tsan_write(void *addr, size_t mem_size, void *rip) { num_writes++; }

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
