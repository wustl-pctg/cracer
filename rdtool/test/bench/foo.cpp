#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cassert>

#include <cilk/cilk.h>
#include "../omrd.h"

#define spawn cilk_spawn
#define sync cilk_sync
#define parfor cilk_for

void bar()
{
  volatile int x = 42;
  for (int i = 0; i < 99999; ++i)
    x++;
}
void foo()
{
  spawn bar();
  bar();
  sync;
  return;
}

int main(int argc, char* argv[])
{
#ifdef RACEDETECT
  __om_disable_checking(); ///@todo find a way to remove this...is it
                           ///even necessary?
#endif

  foo();
  cilk_tool_destroy();

  return 0;
}
