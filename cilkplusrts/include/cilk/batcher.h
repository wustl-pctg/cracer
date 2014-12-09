/** @file batcher.h          -*-C++-*-
 * @author Robert Utterback
 * 
 * @brief Function declarations for the Batcher runtime system.
 */
#ifndef BATCHER_H
#define BATCHER_H

#include <cilk/common.h>

__CILKRTS_BEGIN_EXTERN_C
typedef void (*function_t)(void);
CILK_API(void) cilk_batchify(function_t f);

enum BATCH_ITEM_STATUS { ITEM_WAITING, ITEM_IN_PROGRESS, ITEM_DONE };
typedef int batch_data_t;

struct batch_record {
  function_t        operation;
  void*             ds;
  int            data_size;
  batch_data_t      data;
  enum BATCH_ITEM_STATUS status;
};

struct batch {
  function_t operation;
  void*      ds;
  int     data_size;
  int     num_ops;
  void*      work_array;
  int id;
};

__CILKRTS_END_EXTERN_C

#endif /* BATCHER_H */
