#ifndef _BENCH_OPTIONS_H
#define _BENCH_OPTIONS_H

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

int getOptions(int argc, char **argv, BenchOptions *opt);
void usage();


#endif
