#ifndef __DEBUG_UTIL_H__
#define __DEBUG_UTIL_H__

#include <assert.h>

#ifndef DNDEBUG
#define OM_DEBUG 1
#else
#define OM_DEBUG 0
#endif

// debug_level is a bitmap
//   1 is basic debugging (old level 1)
//   2 is debug the backtrace
enum debug_levels {
    DEBUG_BASIC      = 1,
    DEBUG_BACKTRACE  = 2,
    DEBUG_CALLBACK   = 4,
    DEBUG_MEMORY     = 8
};

__attribute__((noreturn)) 
void die(const char *fmt, ...);
void debug_printf(int level, const char *fmt, ...);

#if OM_DEBUG > 0
static int debug_level = DEBUG_CALLBACK | DEBUG_MEMORY;
//static int debug_level = 0;
#else
static int debug_level = 0;
#endif

/* #if OM_DEBUG > 0 */
/* #define WHEN_OM_DEBUG(stmt) do { stmt } while(0); */
/* #define om_assert(c) \ */
/*     do { if (!(c)) { die("%s:%d assertion failure: %s\n", \ */
/*                         __FILE__, __LINE__, #c);} } while (0) */
/* #define locked_assert(c) */
/* // A deadlock involved getting the worker deque lock in batchify */
/* // prevents using this */
/*   /\* do { pthread_spin_lock(&g_worker_mutexes[self]); \ *\/ */
/*   /\* assert(c); \ *\/ */
/*   /\* pthread_spin_unlock(&g_worker_mutexes[self]); \ *\/ */
/*   /\* } while (0) *\/ */
/* #else */
#define WHEN_OM_DEBUG(stmt)
#define om_assert(c)
#define locked_assert(c)
/* #endif */

#if OM_DEBUG > 0
// debugging assert to check that the tool is catching all the runtime events
// that are supposed to match up (i.e., has event begin and event end)
enum EventType_t { ENTER_FRAME = 1, ENTER_HELPER = 2, SPAWN_PREPARE = 3,
                   DETACH = 4, CILK_SYNC = 5, LEAVE_FRAME_OR_HELPER = 6,
                   RUNTIME_LOOP = 7, NONE = 8 };
#endif



/// @todo temporarily disabled...why did I do this?
#undef OM_DEBUG
#if OM_DEBUG
#define DBG_TRACE(level,...) debug_printf(level, __VA_ARGS__)
#else
#define DBG_TRACE(level,...)
#endif

#endif
