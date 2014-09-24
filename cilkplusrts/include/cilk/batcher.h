/** @file batcher.h          -*-C++-*-
 * @author Robert Utterback
 * 
 * @brief Function declarations for the Batcher runtime system.
 */
#ifndef BATCHER_H
#define BATCHER_H

#include <cilk/common.h>

typedef void (*function_t)(void);

__CILKRTS_BEGIN_EXTERN_C
CILK_API(void) cilk_batchify(function_t f);
__CILKRTS_END_EXTERN_C

#endif /* BATCHER_H */
