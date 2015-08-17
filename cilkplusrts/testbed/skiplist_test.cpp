#include <stdio.h>
#include <string.h>

#include <cilk/cilk.h>
#include <cilk/batcher.h>

#include "timer.h"
#include "skiplist.h"

#define DEFAULT_INIT_SIZE 20
#define DEFAULT_NUM_OPS 100000
#define SEQUENTIAL_VALS 0

int* g_insert_vals;

SkipList* set_up(size_t initial_size, size_t num_ops)
{
  SkipList* ds = &slist;

  // Initialize the skip list.
  initList();

  for (int i = -1 * (int)initial_size; i < 0; ++i) {
    int val = SEQUENTIAL_VALS ? i : rand();
    insertNode(val);
  }

  // Prepare our array of random numbers.
  g_insert_vals = (int*) malloc(sizeof(int) * num_ops);
  if (!g_insert_vals)
    fprintf(stderr, "insufficient memory (vals)\n");

  for (int i = 0; i < num_ops; ++i) {
    int val = SEQUENTIAL_VALS ? i : rand();
    g_insert_vals[i] = val;
  }
  return ds;
}

void run_insert_benchmark(SkipList* ds, int size)
{
  cilk_for(int i = 0; i < size; ++i) {
    SkipList_insert(g_insert_vals[i]);
  }
}

int main(int argc, char* argv[])
{
  int num_ops = (argc == 2) ? atoi(argv[1]) : DEFAULT_NUM_OPS;
  int initial_size = (argc == 3) ? atoi(argv[2]) : DEFAULT_INIT_SIZE;

  SkipList* ds = set_up(initial_size, num_ops);

  startTimer();
  run_insert_benchmark(ds, num_ops);
  stopTimer();

  printf("Time: %lf", getWallTime());
  printf(" | Size: %d\n", get_size());

  return 0;
}
