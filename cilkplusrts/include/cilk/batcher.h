/** @file batcher.h          -*-C++-*-
 * @author Robert Utterback
 * 
 * @brief Function declarations for the Batcher runtime system.
 */
#ifndef BATCHER_H
#define BATCHER_H

#include <cilk/common.h>
#include <stddef.h>
#include <stdlib.h>

__CILKRTS_BEGIN_EXTERN_C
typedef int batch_data_t;
typedef void(*batch_function_t)(void* ds, void* data, size_t size, void* results);
//typedef size_t unsigned int;
CILK_API(void) cilk_batchify(batch_function_t f, void* ds,
                             batch_data_t data, size_t data_size);
CILK_API(void) __cilkrts_c_terminate_batch();
CILK_API(void) __cilkrts_c_start_batch();

enum BATCH_ITEM_STATUS { ITEM_WAITING, ITEM_IN_PROGRESS, ITEM_DONE };


struct batch_record {
  batch_function_t       operation;
  void*                  ds;
  size_t           data_size;
  batch_data_t           data;
  volatile enum BATCH_ITEM_STATUS status;
};

struct batch {
  batch_function_t operation;
  void*            ds;
  size_t    data_size;
  size_t     num_ops;
  void*            work_array;
  int              id;
};

__CILKRTS_END_EXTERN_C

#endif /* BATCHER_H */
