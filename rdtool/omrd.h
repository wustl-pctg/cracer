#ifndef _OMRD_H
#define _OMRD_H

#include <pthread.h>
#include <malloc.h> // for memalign

extern "C" void cilk_tool_print(void);
extern "C" void cilk_tool_destroy(void);
extern "C" void __om_enable_checking(); 
extern "C" void __om_disable_checking();
extern "C" void __om_disable_instrumentation();

#define PADDING char pad[(64 - sizeof(pthread_spinlock_t))]

typedef struct local_mut_s {
  pthread_spinlock_t mut;
  PADDING;
} local_mut;

// If you try to grab one of these while owning a worker lock, e.g. in
// cilk_steal_success, you will deadlock if there is a batch relabel
// in progress.
//pthread_spinlock_t* g_worker_mutexes; 
//local_mut* g_worker_mutexes;

#endif // _OMRD_H
