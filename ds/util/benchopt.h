#ifndef _BENCH_OPT_H
#define _BENCH_OPT_H

typedef struct
{
	int verbose;
	int noBatch;
	int noLock;
	int operations;
	int iterations;
	int dedicated;
	int special;
  int raw;
  int seq;
  int initial_size;
} BenchOptions;

int get_options(int argc, char **argv, BenchOptions* opt);
void usage();


#endif
