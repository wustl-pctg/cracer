#ifndef _BENCH_H
#define _BENCH_H

#include <iostream>
#include <chrono>

// This is a hack. I'm sharing the rd.h file between tests and the
// actual library source, so this is a separate file. Ideally the two
// should be split, and this file should be merged with one in
// $(BASE_DIR)/include.

#if defined RACEDETECT || defined INSERTSONLY
#define RD_ENABLE __om_enable_checking()
#define RD_DISABLE __om_disable_checking()

#include "rd.h"
#include "../src/print_addr.h"
#else
#define RD_ENABLE
#define RD_DISABLE
#endif

typedef struct FrameData_s {
  void* current_english;
  void* current_hebrew;
  void* cont_english;
  void* cont_hebrew;
  void* sync_english;
  void* sync_hebrew;
  uint32_t flags;
  char func_name[32];
} FrameData_t;
extern FrameData_t* get_frame();
#include "../../../cilkplusrts/include/internal/abi.h" /// For get_tls_worker(), remove later
#define ENTER_FRAME int last = __cilkrts_get_tls_worker()->self; strcpy(get_frame()->func_name,__func__)
#define CORRECT_FRAME assert(strncmp(get_frame()->func_name, __func__, 2) == 0); last = __cilkrts_get_tls_worker()->self

#ifdef INSERTSONLY
extern "C" void __tsan_init();
void init_inserts_only(void) __attribute__((constructor));
void init_inserts_only(void)
{
  __tsan_init();
  RD_DISABLE;
  __om_disable_instrumentation();
}

void destroy_inserts_only(void) __attribute__((destructor));
void destroy_inserts_only(void)
{
  cilk_tool_destroy();
}
#endif

#ifdef RACEDETECT
void destroy_rd(void) __attribute__((destructor));
void destroy_rd(void)
{
  assert(get_num_races_found() == 0);
  cilk_tool_destroy();
}
#endif

/* #ifdef STATS */
/* #include "stattool.h" */
/* void print_final_stats(void) __attribute__((destructor)); */
/* void print_final_stats(void) {} */
/* #endif */

#endif
