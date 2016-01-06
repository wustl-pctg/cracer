#include <internal/abi.h>
#include "stat_util.h"

__thread __cilkrts_worker* t_worker = NULL;
__thread int self = -1;

#if STATS > 1
double *g_timing_events;//[NUM_INTERVAL_TYPES];
struct timespec *g_timing_event_starts;//[NUM_INTERVAL_TYPES];
struct timespec *g_timing_event_ends;//[NUM_INTERVAL_TYPES];
struct timespec EMPTY_TIME_POINT;
int g_nproc = 0;
#endif

#if STATS > 0
unsigned long g_num_relabels = 0;
unsigned long g_num_inserts = 0;
unsigned long g_relabel_size = 0;
#endif
