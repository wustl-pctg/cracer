#ifndef _COMMON_H
#define _COMMON_H

#include <stdlib.h>

#ifdef CILK
#define cilk cilk
#define spawn spawn
#define sync sync
#else
#define cilk
#define spawn
#define sync
#endif

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// Note: this will only work in gcc!

#define PARFOR_START(name) void parfor_##name(int begin, int end)  \
  { \
    int index; \
    if ((end - begin) <= 8) { \
    for (index = begin; index <= end; ++index) { \

#define PARFOR_END(name,b,e) }                  \
    } else { \
      index = begin + (end - begin) / 2; \
      parfor_##name(begin, index);              \
      parfor_##name(index + 1, end); \
    } \
  } \
  parfor_##name(b,e);

#endif // _COMMON_H
