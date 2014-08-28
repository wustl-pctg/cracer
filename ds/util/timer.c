#include <stdlib.h>

#include "timer.h"

void timer_start()
{
  _global_timer.stopped = 0;
	gettimeofday(&_global_timer.wall_begin, NULL);
	/* getrusage(RUSAGE_SELF, &_timer_usage); */
	/* _timer_userBegin = _timer_usage.ru_utime; */
	/* _timer_kernelBegin = _timer_usage.ru_stime; */
}

void timer_stop()
{
  _global_timer.stopped = 1;
	gettimeofday(&_global_timer.wall_end, NULL);
	/* getrusage(RUSAGE_SELF, &_timer_usage); */
	/* _timer_userEnd = _timer_usage.ru_utime; */
	/* _timer_kernelEnd = _timer_usage.ru_stime; */
}

msec get_elapsed_time(struct timeval *start, struct timeval *end)
{
	return (msec)(end->tv_sec*1000000.0 + end->tv_usec -
                (start->tv_sec*1000000.0 + start->tv_usec))/1000;

}

msec timer_get_wall_time()
{
  if (!_global_timer.stopped) timer_stop();
	return get_elapsed_time(&_global_timer.wall_begin, &_global_timer.wall_end);
}

/* inline double timer_get_wall_time() */
/* { */
/* 	return getElapsedTime(&_timer_userBegin, &_timer_userEnd) + */
/* 		getElapsedTime(&_timer_kernelBegin, &_timer_kernelEnd); */
/* } */
