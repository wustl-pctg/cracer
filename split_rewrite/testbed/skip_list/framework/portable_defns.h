#ifndef __PORTABLE_DEFNS_H__
#define __PORTABLE_DEFNS_H__

#include "intel64_defns.h"

#if defined(WIN32) || defined(WIN64)
	#define __thread__ __declspec(thread)
#else
	#define __thread__ __thread
#endif


#include <time.h>
#include <math.h>
#include <signal.h>
//#include <sstream>
#include <stdio.h>
#include <limits.h>
#include <sys/time.h>
#include <stdlib.h>


#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef null
#define null (0)
#endif

#define final const
#define boolean char

#define System_out_println(x) {fprintf(stdout, x); fprintf(stdout, "\n");}
#define System_err_println(x) {fprintf(stderr, x); fprintf(stderr, "\n");} 
#define System_cout(x) {printf(x); printf("\n");}
#define System_out_format printf

#endif /* __PORTABLE_DEFNS_H__ */
