#ifndef __CMDR_C_FRAMEWORK__
#define __CMDR_C_FRAMEWORK__


////////////////////////////////////////////////////////////////////////////////
//include directives
////////////////////////////////////////////////////////////////////////////////
#include <time.h>
#include <sched.h>
#include "portable_defns.h"

/////////////////////////////////////////////////////////////////////////////
//system 
/////////////////////////////////////////////////////////////////////////////
#define true 1

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

typedef struct Volatile_u32 {
  volatile _u32 _value;
} Volatile_u32;

inline_ Volatile_u32* new_Volatile_u32 (const _u32 value) {
  Volatile_u32* new_volatile;
  new_volatile = (Volatile_u32*)malloc(sizeof(Volatile_u32));
  new_volatile->_value = value;
  write_barrier();
  return new_volatile;
}

inline_ _u32 Volatile_u32_get(Volatile_u32* volatile_u32) {
  read_barrier();
  return volatile_u32->_value;
}

inline_ void Volatile_u32_set(Volatile_u32* volatile_u32, const _u32 value) {
  //printf("volatile_u32: %s\n", volatile_u32);
  volatile_u32->_value = value;
  write_barrier();  
}

typedef struct Volatile_tick_t {
  volatile tick_t _value;
} Volatile_tick_t;

inline_ Volatile_tick_t* new_Volatile_tick_t (const tick_t value) {
  Volatile_tick_t* new_volatile;
  new_volatile = (Volatile_tick_t*)malloc(sizeof(Volatile_tick_t));
  new_volatile->_value = value;
  write_barrier();
  return new_volatile;
}

inline_ tick_t Volatile_tick_t_get(Volatile_tick_t* volatile_tick_t) {
  read_barrier();
  return volatile_tick_t->_value;
}

inline_ void Volatile_tick_t_set
    (Volatile_tick_t* volatile_tick_t, const tick_t value) 
{
  volatile_tick_t->_value = value;
  write_barrier();   
}

typedef struct AtomicInteger {
  Volatile_u32* _value;
} AtomicInteger;

AtomicInteger* new_AtomicInteger(int value) {
  AtomicInteger* atom = (AtomicInteger*)malloc(sizeof(AtomicInteger));
  atom->_value = (Volatile_u32*)malloc(sizeof(Volatile_u32));
  Volatile_u32_set(atom->_value, value);
  return atom;
}

inline_ int AtomicInteger_get(AtomicInteger* atom) {
  return Volatile_u32_get(atom->_value);
}

inline_ int AtomicInteger_getAndIncrement(AtomicInteger* atom) {
  /*do {
    int value = Volatile_u32_get(atom->_value);
    if (value == CAS32(&atom->_value->_value, value, value+1)) {
      return value;
    }
  } while(true);*/
  return (atom->_value->_value++);
}

inline_ static tick_t System_read_cpu_ticks() {return RDTICK();}

typedef struct Random {
  double u[97];
  double c, cd, cm, s, t;
  int i97, j97, ii, i, j, k, l, jj, m, ij, kl;
} Random;

Random* new_Random() {
  Random* n_random;
  n_random = (Random*) malloc (sizeof(Random));
  n_random->ij = (System_read_cpu_ticks()*time(0))%31329;
  n_random->kl = (System_read_cpu_ticks()*time(0))%30082;
  if (n_random->ij <0 || n_random->ij >3132 || n_random->kl <0 
      || n_random->kl >30081) 
  {
    n_random->ij = 1802;
    n_random->kl = 9373;
  }
  n_random->i = (n_random->ij / 177) % 177 + 2;
  n_random->j = (n_random->ij % 177)       + 2;
  n_random->k = (n_random->kl / 169) % 178 + 1;
  n_random->l = (n_random->kl % 169);
  
  for (n_random->ii=0; n_random->ii<97; n_random->ii++) {
    n_random->s = 0.0;
    n_random->t = 0.5;
    for (n_random->jj=0; n_random->jj<24; n_random->jj++) {
      n_random->m = (((n_random->i*n_random->j) % 179) * n_random->k) % 179;
      n_random->i = n_random->j;
      n_random->j = n_random->k;
      n_random->k = n_random->m;
      n_random->l = (53 * n_random->l + 1) % 169;
      if (((n_random->l * n_random->m % 64)) >= 32)
        n_random->s += n_random->t;
      n_random->t *= 0.5;
    }
    n_random->u[n_random->ii] = n_random->s;
  }
  n_random->c   = 362436.0 / 16777216.0;
  n_random->cd  = 7654321.0 / 16777216.0;
  n_random->cm  = 16777213.0 / 16777216.0;
  n_random->i97 = 97;
  n_random->j97 = 33;
  
  return n_random;
}

inline_ double Random_nextUniform(Random* in) {
  double uni;

  uni = in->u[in->i97-1] - in->u[in->j97-1];
  if (uni <= 0.0)
    ++uni;
  in->u[in->i97-1] = uni;
  in->i97--;
  if (in->i97 == 0)
    in->i97 = 97;
  in->j97--;
  if (in->j97 == 0)
    in->j97 = 97;
  in->c -= in->cd;
  if (in->c < 0.0)
    in->c += in->cm;
  uni -= in->c;
  if (uni < 0.0)
    uni++;
  return uni;
}

inline_ int Random_nextInt(Random* in, int lower, int upper) {
  int tmp;
  tmp = ((unsigned int)(Random_nextUniform(in) * (upper - lower)) + lower);
  if (tmp >= upper)
    tmp = upper-1;
  return tmp;
}

inline_ unsigned int Random_nextUInt(Random* in, int upper) {
  return Random_nextInt(in, 0, upper);
}

inline_ static _u64 Random_getSeed (){
  return (time(0)*System_read_cpu_ticks()) ^ (clock()*System_read_cpu_ticks());
}

inline_ static _u64 Random_getRandom(_u64 seed) {
  if(0==seed)
    seed = Random_getSeed();
  seed = seed * 196314165 + 907633515;
  return seed;
}

inline_ static int Random_getRandomInt(_u64 volatile* seed, int inValue) {
  // Return a random number between 0 and value
  if(0==*seed)
    *seed = Random_getSeed();
  *seed = *seed * 196314165 + 907633515;
  if (inValue>0)
    return (int)(*seed%(inValue));
  else
    return (0);
}

static void framework_sleep(const long millis) {
  struct timespec delta;
  const long num_sec = millis/1000;
  delta.tv_nsec = (millis - num_sec*1000) * 1000000;
  delta.tv_sec = num_sec;
  nanosleep(&delta, 0);
}

static void yield() {
  //Sleep(0);
  sched_yield();
}

#endif

