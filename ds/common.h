#ifdef CILK
#define CILK_ONLY(code) code
#else
#define CILK_ONLY(code)
#endif
