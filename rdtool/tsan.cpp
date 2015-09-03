
extern "C" {
static bool TOOL_INITIALIZED = false;
static bool check_enable_instrumentation = true;

// When either is set to false, no errors are output
static bool instrumentation = false;
// needs to be reentrant due to reducer operations; 0 means checking
static int checking_disabled = 0;

static void tsan_destroy(void) {}
void __tsan_init()
{
  if(TOOL_INITIALIZED) return;

  atexit(tsan_destroy);
  //init_internal();
  // moved this later when we enter the first Cilk frame
  // cilksan_init();
  // enable_instrumentation();
  TOOL_INITIALIZED = true;
}

void __tsan_func_entry(void *pc) { }
void __tsan_func_exit() {}
void __tsan_vptr_read(void **vptr_p) {}
void __tsan_vptr_update(void **vptr_p, void *new_val) {}

// get_user_code_rip calls the system backtrace to walk the stack to obtain 
// list of return addresses sitting on the cucrrent stack.  
// Buffer[0] always stores the return address for the backtrace call.  
// Where we find the rip for the access performed in the user program 
// depends on how deeply the backtrace call is nested, which is provided
// as an arg to get_user_code_rip. 
/*
static inline void *get_user_code_rip(int depth) {
    disable_checking();
    void *buffer[depth];
    int res = backtrace(buffer, depth);
    cilksan_assert(res == depth);
    enable_checking();
    return buffer[depth-1];
} */

// Assuming tsan_read/write is inlined, the stack should look like this:
//
// -------------------------------------------
// | user func that is about to do a memop   |
// -------------------------------------------
// | __tsan_read/write[1-16]                 |
// -------------------------------------------
// | backtrace (assume __tsan_read/write and | 
// |            get_user_code_rip is inlined)|
// -------------------------------------------
//
// In the user program, __tsan_read/write[1-16] are inlined
// right before the corresponding read / write in the user code.
// the return addr of __tsan_read/write[1-16] is the rip for the read / write

static inline void tsan_read(void *addr, size_t size, void *rip)
{

}

static inline void tsan_write(void *addr, size_t size, void *rip)
{

}

void __tsan_read1(void *addr) { tsan_read(addr, 1, __builtin_return_address(0)); }
void __tsan_read2(void *addr) { tsan_read(addr, 2, __builtin_return_address(0)); }
void __tsan_read4(void *addr) { tsan_read(addr, 4, __builtin_return_address(0)); }
void __tsan_read8(void *addr) { tsan_read(addr, 8, __builtin_return_address(0)); }
void __tsan_read16(void *addr) { tsan_read(addr, 16, __builtin_return_address(0)); }
void __tsan_write1(void *addr) { tsan_write(addr, 1, __builtin_return_address(0)); }
void __tsan_write2(void *addr) { tsan_write(addr, 2, __builtin_return_address(0)); }
void __tsan_write4(void *addr) { tsan_write(addr, 4, __builtin_return_address(0)); }
void __tsan_write8(void *addr) { tsan_write(addr, 8, __builtin_return_address(0)); }
void __tsan_write16(void *addr) { tsan_write(addr, 16, __builtin_return_address(0)); }

}
