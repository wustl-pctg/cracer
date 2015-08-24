#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cassert>

#include <cilk/cilk.h>
//#include "../../om/om.h"
//#include <racedetect.h>

#define spawn cilk_spawn
#define sync cilk_sync
#define parfor cilk_for

int fib(int n)
{
  //  std::cout << "fib(" << n << ")\n";
  if (n < 2) return n;
  int x, y;
  x = spawn fib(n - 1);
  y = spawn fib(n - 2);
  sync;
  return x + y;
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: fib <n>" << std::endl;
    exit(1);
  }

  int n = std::atoi(argv[1]);
  auto start = std::chrono::high_resolution_clock::now();
  int result = fib(n);
  auto end = std::chrono::high_resolution_clock::now();

  // Run a second time to make sure we can handle entering separate
  // cilk functions
  int result2 = fib(n);
  assert(result == result2);

  //std::cout << "Result: " << result << std::endl;
  std::cout << "Time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << " ms."
            << std::endl;

  return 0;
}
