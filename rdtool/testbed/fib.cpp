#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cassert>
#include <unistd.h> // for sbrk

#include <cilk/cilk.h>
#include "../omrd.h"

#define spawn cilk_spawn
#define sync cilk_sync
#define parfor cilk_for

int fib(int n);

int bar(int n)
{
  spawn fib(n);
  sync;
  return 0;
}

int foo(int n)
{
  int x = spawn fib(n);
  sync;
  bar(n);
  return x;
}

int fib(int n)
{
  //std::cout << "fib(" << n << ")\n";
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
  // int result = spawn foo(n);
  // sync;
  int result = fib(n);

  auto end = std::chrono::high_resolution_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
  cilk_tool_destroy();

  return 0;
}
