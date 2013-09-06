#ifndef __CMDR_C_FRAMEWORK__
#define __CMDR_C_FRAMEWORK__


////////////////////////////////////////////////////////////////////////////////
//include directives
////////////////////////////////////////////////////////////////////////////////
#include "portable_defns.h"

/////////////////////////////////////////////////////////////////////////////
//system 
/////////////////////////////////////////////////////////////////////////////
inline_ static tick_t read_cpu_ticks() {return RDTICK();}

inline_ static tick_t currentTimeMillis() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return 1000*(tp.tv_sec) + (tp.tv_usec/1000.0);
}

/*inline_ static void exit(const int exit_code) {
	exit(exit_code);
}*/

/*inline_ static double ceil(double a) {return ::ceil(a);}
inline_ static double floor(double a) {return ::floor(a);}*/

inline_ static int Max(final int a, final int b) {if(a >= b) return a; else return b;}
inline_ static int Min(final int a, final int b) {if(a <= b) return a; else return b;}


static const int CACHELINE_SIZE = CACHE_LINE_SIZE;

inline_ static void* byte_malloc(const size_t size)	{return malloc(size);}
inline_ static void  byte_free(void* mem)					{free(mem);}

inline_ static void* byte_aligned_malloc(const size_t size) 
  {return ALIGNED_MALLOC(size, CACHELINE_SIZE);}
/*inline_ static void* byte_aligned_malloc
  (const size_t size, const size_t alignment) 
  {return ALIGNED_MALLOC(size,alignment);}*/

inline_ static void  byte_aligned_free(void* mem)	{ALIGNED_FREE(mem);}

inline_ static void  read_write_barrier()	{ MB();	}
inline_ static void  write_barrier()		{ WMB(); }
inline_ static void  read_barrier()			{ RMB(); }

/*inline_ static _u32  compare_and_set(_u32 volatile* _a, _u32 _o, _u32 _n)		
  {return CAS32(_a,_o,_n);}
inline_ static void* compare_and_set(void* volatile * _a, void* _o, void* _n)	
  {return (void*)CASPO(_a,_o,_n);}
inline_ static _u64  compare_and_set(_u64 volatile* _a, _u64 _o, _u64 _n)		
  {return CAS64(_a,_o,_n);}

inline_ static _u32  exchange_and_set(_u32 volatile *   _a, _u32  _n)	
  {return SWAP32(_a,_n);}
inline_ static void* exchange_and_set(void * volatile * _a, void* _n)	
  {return SWAPPO(_a,_n);}*/

typedef struct Volatile_u64 {
  volatile _u64 _value;
} Volatile_u64;

inline_ Volatile_u64* new_Volatile_u64 (const _u64 value) {
  Volatile_u64* new_volatile;
  new_volatile = (Volatile_u64*)malloc(sizeof(Volatile_u64));
  new_volatile->_value = value;
  write_barrier();
  return new_volatile;
}

inline_ _u64 Volatile_u64_get(Volatile_u64* volatile_u64) {
  read_barrier();
  return volatile_u64->_value;
}

inline_ void Volatile_u64_set(Volatile_u64* volatile_u64, const _u64 value) {
  volatile_u64->_value = value;
  write_barrier(); 
  
}
#endif

