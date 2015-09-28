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

extern __thread int self;
extern "C" void cilk_tool_set(int);

int fib(int n);

int bar(int n)
{
  int real = fib(n);
  int x = spawn fib(10);
  int y = fib(9);
  sync;
  return real;
}

int foo(int n)
{
  int real = bar(n);
  int x = spawn fib(10);
  int y = fib(9);
  sync;
  return real;
}

int start_fib(int n)
{
  int real = spawn foo(n);
  int x = spawn fib(10);
  int y = fib(9);
  sync;
  return real;
}

int fib(int n)
{
  cilk_tool_set(n);
  if (n < 2) return n;
  int x, y;
  x = spawn fib(n - 1);
  y = fib(n - 2);
  sync;
  return x + y;
}

int main(int argc, char* argv[])
{
#ifdef RACEDETECT
  __om_disable_checking();
#endif
  if (argc != 2) {
    std::cerr << "Usage: fib <n>" << std::endl;
    exit(1);
  }

  int n = std::atoi(argv[1]);
  auto start = std::chrono::high_resolution_clock::now();
  //  int result = start_fib(n);
  int result = fib(n);

  auto end = std::chrono::high_resolution_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
  cilk_tool_destroy();

  return 0;
}
