#ifndef _BENCH_H
#define _BENCH_H

#include <iostream>
#include <chrono>

// This is a hack. I'm sharing the rd.h file between tests and the
// actual library source, so this is a separate file. Ideally the two
// should be split, and this file should be merged with one in
// $(BASE_DIR)/include.

#ifdef RACEDETECT
#define RD_ENABLE __om_enable_checking()
#define RD_DISABLE __om_disable_checking()

#include "rd.h"
#include "../src/print_addr.h"
#else
#define RD_ENABLE
#define RD_DISABLE
#endif

#ifdef STATS
#include "stattool.h"
void __stattool_print_info() {} // @todo fix this hack
#endif

#endif
