#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cassert>

#include <cilk/cilk.h>
#include "rd.h"

#define spawn cilk_spawn
#define sync cilk_sync
#define parfor cilk_for

int fib(int n)
{
  if (n < 2) return n;
  int x, y;
  x = spawn fib(n - 1);
  y = fib(n - 2);
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

  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
  cilk_tool_destroy();

  return 0;
}
