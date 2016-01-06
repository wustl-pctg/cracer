#ifndef _STAT_UTIL_H
#define _STAT_UTIL_H

extern __thread __cilkrts_worker* t_worker;
extern __thread int self;

typedef enum {
  TOOL = 0,
  TOOL_INSERT,
  FAST_PATH,
  SLOW_PATH,
  /* OM_INSERT, */
  /* OM_RELABEL, */
  /* SHADOW_STACK_MANIP, */
  /* HEAVY_ARRAY_MANIP, */
  /* BLIST_SPLIT, */
  /* OM_UPDATE_SIZES, */
  /* OM_REBALANCE, */
  /* OM_BUILD_ARRAY_OF_LEAVES, */
  /* OM_REBUILD, */
  /* OM_REBUILD_MALLOC, */
  /* OM_MALLOC, */
  NUM_INTERVAL_TYPES,
} interval_types;

#if STATS > 1
#include <time.h>
extern double *g_timing_events;//[NUM_INTERVAL_TYPES];
extern struct timespec *g_timing_event_starts;//[NUM_INTERVAL_TYPES];
extern struct timespec *g_timing_event_ends;//[NUM_INTERVAL_TYPES];
extern struct timespec EMPTY_TIME_POINT;
extern int g_nproc;
#define ZERO_DURATION 0
#define INTERVAL_CAST(x) x
#define TIDX(i) ((g_nproc * self) + (i))
#define RDTOOL_INTERVAL_BEGIN(i) \
  clock_gettime(CLOCK_REALTIME, &g_timing_event_starts[TIDX(i)]);
#define RDTOOL_INTERVAL_END(i) \
  clock_gettime(CLOCK_REALTIME, &g_timing_event_ends[TIDX(i)]);         \
  g_timing_events[TIDX(i)] += (g_timing_event_ends[TIDX(i)].tv_sec - g_timing_event_starts[TIDX(i)].tv_sec) * 1000.0 + \
  (g_timing_event_ends[TIDX(i)].tv_nsec - g_timing_event_starts[TIDX(i)].tv_nsec) / 1000000.0; \
  g_timing_event_starts[TIDX(i)].tv_sec = 0; \
  g_timing_event_starts[TIDX(i)].tv_nsec = 0;
#else // no stats
#define RDTOOL_INTERVAL_BEGIN(i)
#define RDTOOL_INTERVAL_END(i)
#endif // if STATS > 1

#if STATS > 0
extern unsigned long g_num_relabels;
extern unsigned long g_num_inserts;
extern unsigned long g_relabel_size;
#endif

#if STATS > 0
#define RD_STATS(...) __VA_ARGS__
#else
#define RD_STATS(...)
#endif


#endif // _STAT_UTIL_H
