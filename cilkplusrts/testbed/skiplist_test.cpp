#include <stdio.h>
#include <string.h>

#include <cilk/cilk.h>
#include <cilk/batcher.h>

#include "timer.h"
//#include "util/benchOptions.h"
#include "skiplist.h"

#define INIT_SIZE 20000

//BenchOptions g_options;
int* vals;

SkipList* set_up(size_t initial_size, size_t num_ops)
{
  int i;
  SkipList* ds = &slist;

  // Initialize the skip list.
  initList();

  for (i = -1*(int)initial_size; i < 0; i++) {
    insertNode(rand());
    //insertNode(i);
  }

  // Prepare our array of random numbers.
  vals = (int*) malloc(sizeof(int) * num_ops);
  if (!vals)
    fprintf(stderr, "insufficient memory (vals)\n");

  for (i = 0; i < num_ops; i++) {
    vals[i] = rand();;
  }

  return ds;
}

void run_insert_benchmark(SkipList* ds, int size)
{
  cilk_for(int i = 0; i < size; ++i) {
    SkipList_insert(vals[i]);
  }
}

int main(int argc, char* argv[])
{
  SkipList* ds;
  int num_ops = 10000;
  int initial_size = 1000000;

  ds = set_up(initial_size, num_ops);

  startTimer();
  run_insert_benchmark(ds, num_ops);
  // for (int i = 0; i < num_ops; ++i) {
  //   insertNode(vals[i]);
  // }
  stopTimer();

  printf("Time: %lf",getWallTime());
  printf(" | Size: %d\n", get_size());

  return 0;
}
