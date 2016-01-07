#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cilk/cilk.h>
#include "bench.h"

// int * count;

/* 
 * nqueen  4 = 2 
 * nqueen  5 = 10 
 * nqueen  6 = 4 
 * nqueen  7 = 40 
 * nqueen  8 = 92 
 * nqueen  9 = 352 
 * nqueen 10 = 724
 * nqueen 11 = 2680 
 * nqueen 12 = 14200 
 * nqueen 13 = 73712 
 * nqueen 14 = 365596 
 * nqueen 15 = 2279184 
 */

/*
 * <a> contains array of <n> queen positions.  Returns 1
 * if none of the queens conflict, and returns 0 otherwise.
 */
int ok (int n, char *a) {

  int i, j;
  char p, q;

  for (i = 0; i < n; i++) {
    p = a[i];
    for (j = i + 1; j < n; j++) {
      q = a[j];
      if (q == p || q == p - (j - i) || q == p + (j - i))
        return 0;
    }
  }

  return 1;
}

int nqueens (int n, int j, char *a) {

  char *b;
  int i;
  int *count;
  int solNum = 0;

  if (n == j) {
    return 1;
  }

  count = (int *) alloca(n * sizeof(int));
  (void) memset(count, 0, n * sizeof (int));

  for (i = 0; i < n; i++) {

    /***
     * ANGE: strictly speaking, this (alloca after spawn) is frowned 
     * up on, but in this case, this is ok, because b returned by 
     * alloca is only used in this iteration; later spawns don't 
     * need to be able to access copies of b from previous iterations 
     ***/
    b = (char *) alloca((j + 1) * sizeof (char));
    memcpy(b, a, j * sizeof (char));
    b[j] = i;

    if(ok (j + 1, b)) {
      count[i] = cilk_spawn nqueens(n, j + 1, b);
    }
  }
  cilk_sync; 

  for(i = 0; i < n; i++) {
    solNum += count[i];
  }

  return solNum;
}


int main(int argc, char *argv[]) { 
#ifdef INSERTSONLY
  RD_DISABLE;
#endif

  int n = 13;
  char *a;
  int res;

  if (argc < 2) {
    fprintf (stderr, "Usage: %s [<cilk-options>] <n>\n", argv[0]);
    fprintf (stderr, "Use default board size, n = 13.\n");

  } else {
    n = atoi (argv[1]);
    //printf ("Running %s with n = %d.\n", argv[0], n);
  }

  a = (char *) alloca (n * sizeof (char));
  res = 0;

  auto start = std::chrono::high_resolution_clock::now();
  res = nqueens(n, 0, a);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

  if (res == 0) {
    printf ("No solution found.\n");
  } else {
    printf ("Total number of solutions : %d\n", res);
  }

  //  assert(__cilksan_error_count() == 0);
#ifdef RACEDETECT
  assert(get_num_races_found() == 0);
  cilk_tool_destroy();
#endif

#ifdef STATS
  __stattool_print_info();
#endif

  return 0;
}
