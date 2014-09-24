/** @file batcher.h          -*-C++-*-
 * @author Robert Utterback
 * 
 * @brief Function declarations for the Batcher runtime system.
 */
#ifndef BATCHER_H
#define BATCHER_H

#include <cilk/common.h>

__CILKRTS_BEGIN_EXTERN_C
CILK_API(void) cilk_batchify(int test);
__CILKRTS_END_EXTERN_C

#endif /* BATCHER_H */
