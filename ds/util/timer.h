#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <sys/resource.h>

typedef double msec;

struct timer
{
  struct timeval wall_begin;
  struct timeval wall_end;
  //  struct timeval user_begin;
  //  struct timeval user_end;
  //  struct timeval kernel_begin;
  //  struct timeval kernel_end;
  int stopped;
};
typedef struct timer* Timer;

struct timer _global_timer;

void timer_start();
void timer_stop();
double timer_get_wall_time();
/* inline double timer_get_proc_time(); */


#endif
