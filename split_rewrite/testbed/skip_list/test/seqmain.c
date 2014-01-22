#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <signal.h>

#include "./timer.h"
#include "../data_structures/SEQSkipList.h"

#define TOTAL_RAND_NUM 4*1024*1024*8 //TODO: bound this better
#define CAPACITY 4*1024*1024*8 //TODO: bound this better
#define MAX_BENCHMARK_RUNTIME 1
#define NUM_OPS 5000000
#define NUM_THREADS 16 // not balanced for non-power of 2
#define ADD_PROB 100
//#define SIGALRM 14

#define ADD_OP 0
#define DEL_OP 1

static int            _gSeed;
static int            _gNumThreads;
static int            _gInitSize;
static int            _gTotalRandNum;
static int            _gCapacity;
static int            _gAddProb;
static int*           _gRandNumAry;
static int*           _gOpCounts;
static char*          _gRandOpAry;


volatile char         _gStopThreads;


void init_arrays() {
  int iRandNum, iThread;
  _gOpCounts = (int*)malloc(sizeof(int)*_gNumThreads);
  for (iThread = 0; iThread < _gNumThreads; ++iThread) {_gOpCounts[iThread]=0;}
  _gRandNumAry = (int*)malloc(sizeof(int)*_gTotalRandNum);
  for (iRandNum = 0; iRandNum < _gTotalRandNum; ++iRandNum) {
    _gRandNumAry[iRandNum] = (rand()%_gCapacity) + 3;
    if(_gRandNumAry[iRandNum] <= 0 ||
        _gRandNumAry[iRandNum] > (_gCapacity+2))
    {
      fprintf(stderr, "PrepareRandomNumbers: Error random number\n");
      exit(1);
    }
  }
  _gRandOpAry = (char*)malloc(sizeof(char)*_gTotalRandNum);
  for (iRandNum = 0; iRandNum < _gTotalRandNum; ++iRandNum) {
    if (rand()%101 > _gAddProb)
      _gRandOpAry[iRandNum] = ADD_OP;
    else
      _gRandOpAry[iRandNum] = DEL_OP;
  }
}

void startDeadline() {
  sleep(MAX_BENCHMARK_RUNTIME);
  _gStopThreads = 1;
}

void print_results() {
  int i, num_ops = 0;
  for (i=0; i < _gNumThreads; ++i) {
    num_ops += _gOpCounts[i];
  }
  printf("final size: %d\n", size());
  printf("num_ops: %d, runtime(ms): %lf, throughput(ops/ms): %lf\n", 
    num_ops, getWallTime(), num_ops/getWallTime());
  return;
}

void contaminate_memory() {
  int i;
  //contaminate memory manager ..............................................
  for(i=0; i < (1024*64); ++i) {
    void* rand_mem;
    rand_mem = (void*)malloc((rand()%128)+1);
    free(rand_mem);
  }
}

void reset_counters() {
  int iThread;
  for (iThread = 0; iThread < _gNumThreads; ++iThread) {_gOpCounts[iThread]=0;}
}


void abort_benchmark(int num_signal) {
  _gStopThreads=1;
}



void runSeqSkipListBenchmark() {
  int i, rand_index, rand_index_max;
  initList();
  reset_counters();
  if (NUM_OPS + _gInitSize > TOTAL_RAND_NUM) {
    fprintf(stderr, "Capacity too small\n");
    exit(1);
  }

  for (i=0; i < _gInitSize; ++i)
    insertNode(rand()%TOTAL_RAND_NUM+3);  

  contaminate_memory();

  _gStopThreads=0;
  signal(SIGALRM, abort_benchmark);
  alarm(MAX_BENCHMARK_RUNTIME);
  startTimer();
   
  rand_index = 0;
  rand_index_max = NUM_OPS;
  if (NUM_OPS > _gTotalRandNum)
    fprintf(stderr,"rand num array smaller than NUM_OPS\n");
    //printf("level:%d, index:%d\n, max_ops:%d, rand_index:%d, index_max:%d\n", 
    //  level, iLeaf, max_ops, rand_index, rand_index_max);
  while (!_gStopThreads && rand_index < rand_index_max) {
    //printf("doing add\n");
    insertNode(_gRandNumAry[rand_index]);
    ++_gOpCounts[0];
    ++rand_index; 
  }
 
  stopTimer();
  print_results();
}


void runBenchmark(char* benchmark_name) {
  if (!strcmp(benchmark_name, "skiplist")) {
    printf("\n\nSequential Skip list\n-----------------------------\n");
    runSeqSkipListBenchmark();
  } else {
    fprintf(stderr, "Unrecognized benchmark: %s\n", benchmark_name);
  }
}



void usage() {
  fprintf(stderr, "test_dyn <benchmark> <num_threads> [<init size> <capacity>]\n");
}

int main (int argc, char **argv) {
  _gSeed = time(NULL);
  srand(_gSeed);
  _gTotalRandNum = TOTAL_RAND_NUM;
  _gStopThreads = 0;

  if (argc < 3) {
    usage();
    return EXIT_FAILURE;
  }
  _gNumThreads = atoi(argv[2]);
  printf("benchmark: %s, num threads: %d\n", argv[1], atoi(argv[2]));

  if (argc > 3)
    _gInitSize = atoi(argv[3]);
  else 
    _gInitSize = 0;

  if (argc > 4)
    _gCapacity = atoi(argv[4]);
  else
    _gCapacity = CAPACITY;
  
  if (argc > 5)
    _gAddProb = atoi(argv[5]);
  else
    _gAddProb = ADD_PROB;

  init_arrays(); 

  runBenchmark(argv[1]);

  return EXIT_SUCCESS;
}


