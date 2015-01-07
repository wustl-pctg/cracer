#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <sys/resource.h>
typedef double msec;

struct timeval _timer_wallBegin, _timer_wallEnd;
struct timeval _timer_userBegin, _timer_userEnd;
struct timeval _timer_kernelBegin, _timer_kernelEnd;
struct rusage _timer_usage;

void startTimer();
void stopTimer();
inline double getElapsedTime(struct timeval *start, struct timeval *end);
inline double getWallTime();
inline double getProcTime();

void startTimer()
{
	gettimeofday(&_timer_wallBegin, NULL);
	getrusage(RUSAGE_SELF, &_timer_usage);
	_timer_userBegin = _timer_usage.ru_utime;
	_timer_kernelBegin = _timer_usage.ru_stime;
}

void stopTimer()
{
	gettimeofday(&_timer_wallEnd, NULL);
	getrusage(RUSAGE_SELF, &_timer_usage);
	_timer_userEnd = _timer_usage.ru_utime;
	_timer_kernelEnd = _timer_usage.ru_stime;
}

inline double getElapsedTime(struct timeval *start, struct timeval *end)
{
	return (double)(end->tv_sec*1000000.0 + end->tv_usec -
									(start->tv_sec*1000000.0 + start->tv_usec))/1000;

}

inline double getWallTime()
{
	return getElapsedTime(&_timer_wallBegin, &_timer_wallEnd);
}

inline double getProcTime()
{
	return getElapsedTime(&_timer_userBegin, &_timer_userEnd) +
		getElapsedTime(&_timer_kernelBegin, &_timer_kernelEnd);
}

#endif
