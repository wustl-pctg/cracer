# 1 "btree.cilkc"
# 1 "<command-line>"
# 1 "btree.cilkc"
# 1 "btree.cilki"
# 1 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c.h" 1
# 2 "btree.cilki" 2
# 31 "/usr/include/x86_64-linux-gnu/bits/types.h"
typedef unsigned char __u_char;
typedef unsigned short __u_short;
typedef unsigned __u_int;
typedef unsigned long __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned __uint32_t;

typedef long __int64_t;
typedef unsigned long __uint64_t;
# 53 "/usr/include/x86_64-linux-gnu/bits/types.h"
typedef long __quad_t;
typedef unsigned long __u_quad_t;
# 134 "/usr/include/x86_64-linux-gnu/bits/types.h"
typedef unsigned long __dev_t;
typedef unsigned __uid_t;
typedef unsigned __gid_t;
typedef unsigned long __ino_t;
typedef unsigned long __ino64_t;
typedef unsigned __mode_t;
typedef unsigned long __nlink_t;
typedef long __off_t;
typedef long __off64_t;
typedef int __pid_t;
typedef struct ___sue1{int __val[2];}__fsid_t;
typedef long __clock_t;
typedef unsigned long __rlim_t;
typedef unsigned long __rlim64_t;
typedef unsigned __id_t;
typedef long __time_t;
typedef unsigned __useconds_t;
typedef long __suseconds_t;

typedef int __daddr_t;
typedef long __swblk_t;
typedef int __key_t;


typedef int __clockid_t;


typedef void *__timer_t;


typedef long __blksize_t;
# 169 "/usr/include/x86_64-linux-gnu/bits/types.h"
typedef long __blkcnt_t;
typedef long __blkcnt64_t;


typedef unsigned long __fsblkcnt_t;
typedef unsigned long __fsblkcnt64_t;


typedef unsigned long __fsfilcnt_t;
typedef unsigned long __fsfilcnt64_t;

typedef long __ssize_t;
# 184 "/usr/include/x86_64-linux-gnu/bits/types.h"
typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;


typedef long __intptr_t;


typedef unsigned __socklen_t;
# 45 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __loff_t loff_t;
# 49 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __ino_t ino_t;
# 61 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __dev_t dev_t;
# 66 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __gid_t gid_t;
# 71 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __mode_t mode_t;
# 76 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __nlink_t nlink_t;
# 81 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __uid_t uid_t;
# 87 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __off_t off_t;
# 99 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __pid_t pid_t;
# 105 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __id_t id_t;
# 110 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __ssize_t ssize_t;
# 123 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __key_t key_t;
# 60 "/usr/include/time.h"
typedef __clock_t clock_t;
# 76 "/usr/include/time.h"
typedef __time_t time_t;
# 92 "/usr/include/time.h"
typedef __clockid_t clockid_t;
# 104 "/usr/include/time.h"
typedef __timer_t timer_t;
# 137 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __useconds_t useconds_t;
# 141 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __suseconds_t suseconds_t;
# 213 "/usr/lib/gcc/x86_64-linux-gnu/4.7/include/stddef.h"
typedef unsigned long size_t;
# 195 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __attribute__((__mode__ ( __QI__)))int int8_t;
typedef __attribute__((__mode__ ( __HI__)))int int16_t;
typedef __attribute__((__mode__ ( __SI__)))int int32_t;
typedef __attribute__((__mode__ ( __DI__)))int int64_t;


typedef __attribute__((__mode__ ( __QI__)))unsigned u_int8_t;
typedef __attribute__((__mode__ ( __HI__)))unsigned u_int16_t;
typedef __attribute__((__mode__ ( __SI__)))unsigned u_int32_t;
typedef __attribute__((__mode__ ( __DI__)))unsigned u_int64_t;

typedef __attribute__((__mode__ ( __word__)))int register_t;
# 229 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __blksize_t blksize_t;
# 236 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __blkcnt_t blkcnt_t;
# 240 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __fsblkcnt_t fsblkcnt_t;
# 244 "/usr/include/x86_64-linux-gnu/sys/types.h"
typedef __fsfilcnt_t fsfilcnt_t;
# 50 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
typedef unsigned long pthread_t;


typedef union ___sue2
{
  char __size[56];
  long __align;
}
# 53 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
pthread_attr_t;
# 61 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
typedef struct __pthread_internal_list
{
  struct __pthread_internal_list *__prev;
  struct __pthread_internal_list *__next;
}
# 61 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
__pthread_list_t;
# 76 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
typedef union ___sue3
{
  struct __pthread_mutex_s
  {
    int __lock;
    unsigned __count;
    int __owner;

    unsigned __nusers;
# 88 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
    int __kind;

    int __spins;
    __pthread_list_t __list;
# 101 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
  }
# 78 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
  __data;
# 102 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
  char __size[40];
  long __align;
}
# 76 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
pthread_mutex_t;
# 106 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
typedef union ___sue4
{
  char __size[4];
  int __align;
}
# 106 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
pthread_mutexattr_t;
# 115 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
typedef union ___sue6
{
  struct ___sue5
  {
    int __lock;
    unsigned __futex;
    __extension__ unsigned long long __total_seq;
    __extension__ unsigned long long __wakeup_seq;
    __extension__ unsigned long long __woken_seq;
    void *__mutex;
    unsigned __nwaiters;
    unsigned __broadcast_seq;
  }
# 117 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
  __data;
# 128 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
  char __size[48];
  __extension__ long long __align;
}
# 115 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
pthread_cond_t;
# 132 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
typedef union ___sue7
{
  char __size[4];
  int __align;
}
# 132 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
pthread_condattr_t;
# 140 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
typedef unsigned pthread_key_t;
# 144 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
typedef int pthread_once_t;
# 150 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
typedef union ___sue9
{

  struct ___sue8
  {
    int __lock;
    unsigned __nr_readers;
    unsigned __readers_wakeup;
    unsigned __writer_wakeup;
    unsigned __nr_readers_queued;
    unsigned __nr_writers_queued;
    int __writer;
    int __shared;
    unsigned long __pad1;
    unsigned long __pad2;


    unsigned __flags;
  }
# 153 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
  __data;
# 187 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
  char __size[56];
  long __align;
}
# 150 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
pthread_rwlock_t;
# 191 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
typedef union ___sue10
{
  char __size[8];
  long __align;
}
# 191 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
pthread_rwlockattr_t;
# 201 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
typedef volatile int pthread_spinlock_t;
# 206 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
typedef union ___sue11
{
  char __size[32];
  long __align;
}
# 206 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
pthread_barrier_t;
# 212 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
typedef union ___sue12
{
  char __size[4];
  int __align;
}
# 212 "/usr/include/x86_64-linux-gnu/bits/pthreadtypes.h"
pthread_barrierattr_t;
# 150 "/usr/lib/gcc/x86_64-linux-gnu/4.7/include/stddef.h"
typedef long ptrdiff_t;
# 325 "/usr/lib/gcc/x86_64-linux-gnu/4.7/include/stddef.h"
typedef int wchar_t;
# 146 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-sysdep.h"
static __attribute__((__unused__))const char*ident_cilk_sysdep_h= "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/cilk-sysdep.h.in $ $LastChangedBy: bradley $ $Rev: 2311 $ $Date: 2005-07-13 17:38:21 -0400 (Wed, 13 Jul 2005) $";
# 295 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-sysdep.h"
   static inline void CILK_MB(void) {
     __asm__ volatile ("mfence" : : : "memory");
   }




   static inline int Cilk_xchg(volatile int *ptr, int x)
   {
 __asm__ ("xchgl %0,%1" : "=r"(x) : "m"(*(ptr)),"0" (x) : "memory");
 return x;
   }
# 31 "/usr/include/x86_64-linux-gnu/bits/time.h"
struct timeval
  {
    __time_t tv_sec;
    __suseconds_t tv_usec;
  };
# 24 "/usr/include/x86_64-linux-gnu/bits/sigset.h"
typedef int __sig_atomic_t;
# 29 "/usr/include/x86_64-linux-gnu/bits/sigset.h"
typedef struct ___sue13
  {
    unsigned long __val [(1024/ (8* sizeof( unsigned long)))];
  }
# 29 "/usr/include/x86_64-linux-gnu/bits/sigset.h"
__sigset_t;
# 38 "/usr/include/x86_64-linux-gnu/sys/select.h"
typedef __sigset_t sigset_t;
# 120 "/usr/include/time.h"
struct timespec
  {
    __time_t tv_sec;
    long tv_nsec;
  };
# 55 "/usr/include/x86_64-linux-gnu/sys/select.h"
typedef long __fd_mask;
# 65 "/usr/include/x86_64-linux-gnu/sys/select.h"
typedef struct ___sue14
  {
# 70 "/usr/include/x86_64-linux-gnu/sys/select.h"
    __fd_mask fds_bits[1024 / (8* (int) sizeof( __fd_mask))];
# 76 "/usr/include/x86_64-linux-gnu/sys/select.h"
  }
# 65 "/usr/include/x86_64-linux-gnu/sys/select.h"
fd_set;
# 107 "/usr/include/x86_64-linux-gnu/sys/select.h"
extern int select (int __nfds, fd_set *__restrict __readfds,
     fd_set *__restrict __writefds,
     fd_set *__restrict __exceptfds,
     struct timeval *__restrict __timeout);
# 119 "/usr/include/x86_64-linux-gnu/sys/select.h"
extern int pselect (int __nfds, fd_set *__restrict __readfds,
      fd_set *__restrict __writefds,
      fd_set *__restrict __exceptfds,
      const struct timespec *__restrict __timeout,
      const __sigset_t *__restrict __sigmask);
# 65 "/usr/include/x86_64-linux-gnu/sys/time.h"
typedef void *__restrict __timezone_ptr_t;
# 73 "/usr/include/x86_64-linux-gnu/sys/time.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                                                 1)))
# 73 "/usr/include/x86_64-linux-gnu/sys/time.h"
int gettimeofday (struct timeval *__restrict __tv,
    __timezone_ptr_t __tz);
# 93 "/usr/include/x86_64-linux-gnu/sys/time.h"
enum __itimer_which
  {

    ITIMER_REAL= 0,


    ITIMER_VIRTUAL= 1,
# 103 "/usr/include/x86_64-linux-gnu/sys/time.h"
    ITIMER_PROF= 2

  };
# 109 "/usr/include/x86_64-linux-gnu/sys/time.h"
struct itimerval
  {

    struct timeval it_interval;

    struct timeval it_value;
  };
# 122 "/usr/include/x86_64-linux-gnu/sys/time.h"
typedef int __itimer_which_t;
# 127 "/usr/include/x86_64-linux-gnu/sys/time.h"
extern __attribute__((__nothrow__,__leaf__))int getitimer(__itimer_which_t __which,
        struct itimerval *__value);
# 133 "/usr/include/x86_64-linux-gnu/sys/time.h"
extern __attribute__((__nothrow__,__leaf__))int setitimer(__itimer_which_t __which,
        const struct itimerval *__restrict __new,
        struct itimerval *__restrict __old);
# 140 "/usr/include/x86_64-linux-gnu/sys/time.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1)))
# 140 "/usr/include/x86_64-linux-gnu/sys/time.h"
int utimes (const char *__file, const struct timeval *__tvp);
# 268 "/usr/include/unistd.h"
typedef __intptr_t intptr_t;
# 275 "/usr/include/unistd.h"
typedef __socklen_t socklen_t;
# 288 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))int access(const char*__name,int __type);
# 305 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           2)))
# 305 "/usr/include/unistd.h"
int faccessat (int __fd, const char *__file, int __type, int __flag);
# 335 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))__off_t lseek(int __fd,__off_t __offset,int __whence);
# 354 "/usr/include/unistd.h"
extern int close (int __fd);
# 361 "/usr/include/unistd.h"
extern ssize_t read (int __fd, void *__buf, size_t __nbytes);
# 367 "/usr/include/unistd.h"
extern ssize_t write (int __fd, const void *__buf, size_t __n);
# 377 "/usr/include/unistd.h"
extern ssize_t pread (int __fd, void *__buf, size_t __nbytes,
        __off_t __offset);
# 385 "/usr/include/unistd.h"
extern ssize_t pwrite (int __fd, const void *__buf, size_t __n,
         __off_t __offset);
# 418 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int pipe(int*__pipedes);
# 433 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))unsigned alarm(unsigned __seconds);
# 445 "/usr/include/unistd.h"
extern unsigned sleep (unsigned __seconds);
# 470 "/usr/include/unistd.h"
extern int pause (void);
# 474 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1)))
# 474 "/usr/include/unistd.h"
int chown (const char *__file, __uid_t __owner, __gid_t __group);
# 479 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int fchown(int __fd,__uid_t __owner,__gid_t __group);
# 484 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1)))
# 484 "/usr/include/unistd.h"
int lchown (const char *__file, __uid_t __owner, __gid_t __group);
# 492 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (

                                                                           2)))
# 492 "/usr/include/unistd.h"
int fchownat (int __fd, const char *__file, __uid_t __owner,
       __gid_t __group, int __flag);
# 498 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))int chdir(const char*__path);
# 502 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int fchdir(int __fd);
# 512 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))char*getcwd(char*__buf,size_t __size);
# 532 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int dup(int __fd);


extern __attribute__((__nothrow__,__leaf__))int dup2(int __fd,int __fd2);
# 544 "/usr/include/unistd.h"
extern char **__environ;
# 552 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                                                   1, 2)))
# 552 "/usr/include/unistd.h"
int execve (const char *__path, char *const *__argv,
     char *const *__envp);
# 558 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           2)))
# 558 "/usr/include/unistd.h"
int fexecve (int __fd, char *const *__argv,char*const *__envp);
# 564 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1, 2)))
# 564 "/usr/include/unistd.h"
int execv (const char *__path, char *const *__argv);
# 569 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1, 2)))
# 569 "/usr/include/unistd.h"
int execle (const char *__path, const char *__arg,...);
# 574 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1, 2)))
# 574 "/usr/include/unistd.h"
int execl (const char *__path, const char *__arg,...);
# 579 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1, 2)))
# 579 "/usr/include/unistd.h"
int execvp (const char *__file, char *const *__argv);
# 585 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1, 2)))
# 585 "/usr/include/unistd.h"
int execlp (const char *__file, const char *__arg,...);
# 599 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int nice(int __inc);
# 604 "/usr/include/unistd.h"
extern __attribute__((__noreturn__))void _exit(int __status);
# 26 "/usr/include/x86_64-linux-gnu/bits/confname.h"
enum ___sue15
  {
    _PC_LINK_MAX,

    _PC_MAX_CANON,

    _PC_MAX_INPUT,

    _PC_NAME_MAX,

    _PC_PATH_MAX,

    _PC_PIPE_BUF,

    _PC_CHOWN_RESTRICTED,

    _PC_NO_TRUNC,

    _PC_VDISABLE,

    _PC_SYNC_IO,

    _PC_ASYNC_IO,

    _PC_PRIO_IO,

    _PC_SOCK_MAXBUF,

    _PC_FILESIZEBITS,

    _PC_REC_INCR_XFER_SIZE,

    _PC_REC_MAX_XFER_SIZE,

    _PC_REC_MIN_XFER_SIZE,

    _PC_REC_XFER_ALIGN,

    _PC_ALLOC_SIZE_MIN,

    _PC_SYMLINK_MAX,

    _PC_2_SYMLINKS

  };


enum ___sue16
  {
    _SC_ARG_MAX,

    _SC_CHILD_MAX,

    _SC_CLK_TCK,

    _SC_NGROUPS_MAX,

    _SC_OPEN_MAX,

    _SC_STREAM_MAX,

    _SC_TZNAME_MAX,

    _SC_JOB_CONTROL,

    _SC_SAVED_IDS,

    _SC_REALTIME_SIGNALS,

    _SC_PRIORITY_SCHEDULING,

    _SC_TIMERS,

    _SC_ASYNCHRONOUS_IO,

    _SC_PRIORITIZED_IO,

    _SC_SYNCHRONIZED_IO,

    _SC_FSYNC,

    _SC_MAPPED_FILES,

    _SC_MEMLOCK,

    _SC_MEMLOCK_RANGE,

    _SC_MEMORY_PROTECTION,

    _SC_MESSAGE_PASSING,

    _SC_SEMAPHORES,

    _SC_SHARED_MEMORY_OBJECTS,

    _SC_AIO_LISTIO_MAX,

    _SC_AIO_MAX,

    _SC_AIO_PRIO_DELTA_MAX,

    _SC_DELAYTIMER_MAX,

    _SC_MQ_OPEN_MAX,

    _SC_MQ_PRIO_MAX,

    _SC_VERSION,

    _SC_PAGESIZE,


    _SC_RTSIG_MAX,

    _SC_SEM_NSEMS_MAX,

    _SC_SEM_VALUE_MAX,

    _SC_SIGQUEUE_MAX,

    _SC_TIMER_MAX,
# 151 "/usr/include/x86_64-linux-gnu/bits/confname.h"
    _SC_BC_BASE_MAX,

    _SC_BC_DIM_MAX,

    _SC_BC_SCALE_MAX,

    _SC_BC_STRING_MAX,

    _SC_COLL_WEIGHTS_MAX,

    _SC_EQUIV_CLASS_MAX,

    _SC_EXPR_NEST_MAX,

    _SC_LINE_MAX,

    _SC_RE_DUP_MAX,

    _SC_CHARCLASS_NAME_MAX,


    _SC_2_VERSION,

    _SC_2_C_BIND,

    _SC_2_C_DEV,

    _SC_2_FORT_DEV,

    _SC_2_FORT_RUN,

    _SC_2_SW_DEV,

    _SC_2_LOCALEDEF,


    _SC_PII,

    _SC_PII_XTI,

    _SC_PII_SOCKET,

    _SC_PII_INTERNET,

    _SC_PII_OSI,

    _SC_POLL,

    _SC_SELECT,

    _SC_UIO_MAXIOV,

    _SC_IOV_MAX= _SC_UIO_MAXIOV,

    _SC_PII_INTERNET_STREAM,

    _SC_PII_INTERNET_DGRAM,

    _SC_PII_OSI_COTS,

    _SC_PII_OSI_CLTS,

    _SC_PII_OSI_M,

    _SC_T_IOV_MAX,
# 219 "/usr/include/x86_64-linux-gnu/bits/confname.h"
    _SC_THREADS,

    _SC_THREAD_SAFE_FUNCTIONS,

    _SC_GETGR_R_SIZE_MAX,

    _SC_GETPW_R_SIZE_MAX,

    _SC_LOGIN_NAME_MAX,

    _SC_TTY_NAME_MAX,

    _SC_THREAD_DESTRUCTOR_ITERATIONS,

    _SC_THREAD_KEYS_MAX,

    _SC_THREAD_STACK_MIN,

    _SC_THREAD_THREADS_MAX,

    _SC_THREAD_ATTR_STACKADDR,

    _SC_THREAD_ATTR_STACKSIZE,

    _SC_THREAD_PRIORITY_SCHEDULING,

    _SC_THREAD_PRIO_INHERIT,

    _SC_THREAD_PRIO_PROTECT,

    _SC_THREAD_PROCESS_SHARED,


    _SC_NPROCESSORS_CONF,

    _SC_NPROCESSORS_ONLN,

    _SC_PHYS_PAGES,

    _SC_AVPHYS_PAGES,

    _SC_ATEXIT_MAX,

    _SC_PASS_MAX,


    _SC_XOPEN_VERSION,

    _SC_XOPEN_XCU_VERSION,

    _SC_XOPEN_UNIX,

    _SC_XOPEN_CRYPT,

    _SC_XOPEN_ENH_I18N,

    _SC_XOPEN_SHM,


    _SC_2_CHAR_TERM,

    _SC_2_C_VERSION,

    _SC_2_UPE,


    _SC_XOPEN_XPG2,

    _SC_XOPEN_XPG3,

    _SC_XOPEN_XPG4,


    _SC_CHAR_BIT,

    _SC_CHAR_MAX,

    _SC_CHAR_MIN,

    _SC_INT_MAX,

    _SC_INT_MIN,

    _SC_LONG_BIT,

    _SC_WORD_BIT,

    _SC_MB_LEN_MAX,

    _SC_NZERO,

    _SC_SSIZE_MAX,

    _SC_SCHAR_MAX,

    _SC_SCHAR_MIN,

    _SC_SHRT_MAX,

    _SC_SHRT_MIN,

    _SC_UCHAR_MAX,

    _SC_UINT_MAX,

    _SC_ULONG_MAX,

    _SC_USHRT_MAX,


    _SC_NL_ARGMAX,

    _SC_NL_LANGMAX,

    _SC_NL_MSGMAX,

    _SC_NL_NMAX,

    _SC_NL_SETMAX,

    _SC_NL_TEXTMAX,


    _SC_XBS5_ILP32_OFF32,

    _SC_XBS5_ILP32_OFFBIG,

    _SC_XBS5_LP64_OFF64,

    _SC_XBS5_LPBIG_OFFBIG,


    _SC_XOPEN_LEGACY,

    _SC_XOPEN_REALTIME,

    _SC_XOPEN_REALTIME_THREADS,


    _SC_ADVISORY_INFO,

    _SC_BARRIERS,

    _SC_BASE,

    _SC_C_LANG_SUPPORT,

    _SC_C_LANG_SUPPORT_R,

    _SC_CLOCK_SELECTION,

    _SC_CPUTIME,

    _SC_THREAD_CPUTIME,

    _SC_DEVICE_IO,

    _SC_DEVICE_SPECIFIC,

    _SC_DEVICE_SPECIFIC_R,

    _SC_FD_MGMT,

    _SC_FIFO,

    _SC_PIPE,

    _SC_FILE_ATTRIBUTES,

    _SC_FILE_LOCKING,

    _SC_FILE_SYSTEM,

    _SC_MONOTONIC_CLOCK,

    _SC_MULTI_PROCESS,

    _SC_SINGLE_PROCESS,

    _SC_NETWORKING,

    _SC_READER_WRITER_LOCKS,

    _SC_SPIN_LOCKS,

    _SC_REGEXP,

    _SC_REGEX_VERSION,

    _SC_SHELL,

    _SC_SIGNALS,

    _SC_SPAWN,

    _SC_SPORADIC_SERVER,

    _SC_THREAD_SPORADIC_SERVER,

    _SC_SYSTEM_DATABASE,

    _SC_SYSTEM_DATABASE_R,

    _SC_TIMEOUTS,

    _SC_TYPED_MEMORY_OBJECTS,

    _SC_USER_GROUPS,

    _SC_USER_GROUPS_R,

    _SC_2_PBS,

    _SC_2_PBS_ACCOUNTING,

    _SC_2_PBS_LOCATE,

    _SC_2_PBS_MESSAGE,

    _SC_2_PBS_TRACK,

    _SC_SYMLOOP_MAX,

    _SC_STREAMS,

    _SC_2_PBS_CHECKPOINT,


    _SC_V6_ILP32_OFF32,

    _SC_V6_ILP32_OFFBIG,

    _SC_V6_LP64_OFF64,

    _SC_V6_LPBIG_OFFBIG,


    _SC_HOST_NAME_MAX,

    _SC_TRACE,

    _SC_TRACE_EVENT_FILTER,

    _SC_TRACE_INHERIT,

    _SC_TRACE_LOG,


    _SC_LEVEL1_ICACHE_SIZE,

    _SC_LEVEL1_ICACHE_ASSOC,

    _SC_LEVEL1_ICACHE_LINESIZE,

    _SC_LEVEL1_DCACHE_SIZE,

    _SC_LEVEL1_DCACHE_ASSOC,

    _SC_LEVEL1_DCACHE_LINESIZE,

    _SC_LEVEL2_CACHE_SIZE,

    _SC_LEVEL2_CACHE_ASSOC,

    _SC_LEVEL2_CACHE_LINESIZE,

    _SC_LEVEL3_CACHE_SIZE,

    _SC_LEVEL3_CACHE_ASSOC,

    _SC_LEVEL3_CACHE_LINESIZE,

    _SC_LEVEL4_CACHE_SIZE,

    _SC_LEVEL4_CACHE_ASSOC,

    _SC_LEVEL4_CACHE_LINESIZE,
# 499 "/usr/include/x86_64-linux-gnu/bits/confname.h"
    _SC_IPV6= _SC_LEVEL1_ICACHE_SIZE + 50,

    _SC_RAW_SOCKETS,


    _SC_V7_ILP32_OFF32,

    _SC_V7_ILP32_OFFBIG,

    _SC_V7_LP64_OFF64,

    _SC_V7_LPBIG_OFFBIG,


    _SC_SS_REPL_MAX,


    _SC_TRACE_EVENT_NAME_MAX,

    _SC_TRACE_NAME_MAX,

    _SC_TRACE_SYS_MAX,

    _SC_TRACE_USER_EVENT_MAX,


    _SC_XOPEN_STREAMS,


    _SC_THREAD_ROBUST_PRIO_INHERIT,

    _SC_THREAD_ROBUST_PRIO_PROTECT

  };


enum ___sue17
  {
    _CS_PATH,


    _CS_V6_WIDTH_RESTRICTED_ENVS,
# 544 "/usr/include/x86_64-linux-gnu/bits/confname.h"
    _CS_GNU_LIBC_VERSION,

    _CS_GNU_LIBPTHREAD_VERSION,


    _CS_V5_WIDTH_RESTRICTED_ENVS,
# 553 "/usr/include/x86_64-linux-gnu/bits/confname.h"
    _CS_V7_WIDTH_RESTRICTED_ENVS,
# 557 "/usr/include/x86_64-linux-gnu/bits/confname.h"
    _CS_LFS_CFLAGS= 1000,

    _CS_LFS_LDFLAGS,

    _CS_LFS_LIBS,

    _CS_LFS_LINTFLAGS,

    _CS_LFS64_CFLAGS,

    _CS_LFS64_LDFLAGS,

    _CS_LFS64_LIBS,

    _CS_LFS64_LINTFLAGS,


    _CS_XBS5_ILP32_OFF32_CFLAGS= 1100,

    _CS_XBS5_ILP32_OFF32_LDFLAGS,

    _CS_XBS5_ILP32_OFF32_LIBS,

    _CS_XBS5_ILP32_OFF32_LINTFLAGS,

    _CS_XBS5_ILP32_OFFBIG_CFLAGS,

    _CS_XBS5_ILP32_OFFBIG_LDFLAGS,

    _CS_XBS5_ILP32_OFFBIG_LIBS,

    _CS_XBS5_ILP32_OFFBIG_LINTFLAGS,

    _CS_XBS5_LP64_OFF64_CFLAGS,

    _CS_XBS5_LP64_OFF64_LDFLAGS,

    _CS_XBS5_LP64_OFF64_LIBS,

    _CS_XBS5_LP64_OFF64_LINTFLAGS,

    _CS_XBS5_LPBIG_OFFBIG_CFLAGS,

    _CS_XBS5_LPBIG_OFFBIG_LDFLAGS,

    _CS_XBS5_LPBIG_OFFBIG_LIBS,

    _CS_XBS5_LPBIG_OFFBIG_LINTFLAGS,


    _CS_POSIX_V6_ILP32_OFF32_CFLAGS,

    _CS_POSIX_V6_ILP32_OFF32_LDFLAGS,

    _CS_POSIX_V6_ILP32_OFF32_LIBS,

    _CS_POSIX_V6_ILP32_OFF32_LINTFLAGS,

    _CS_POSIX_V6_ILP32_OFFBIG_CFLAGS,

    _CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS,

    _CS_POSIX_V6_ILP32_OFFBIG_LIBS,

    _CS_POSIX_V6_ILP32_OFFBIG_LINTFLAGS,

    _CS_POSIX_V6_LP64_OFF64_CFLAGS,

    _CS_POSIX_V6_LP64_OFF64_LDFLAGS,

    _CS_POSIX_V6_LP64_OFF64_LIBS,

    _CS_POSIX_V6_LP64_OFF64_LINTFLAGS,

    _CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS,

    _CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS,

    _CS_POSIX_V6_LPBIG_OFFBIG_LIBS,

    _CS_POSIX_V6_LPBIG_OFFBIG_LINTFLAGS,


    _CS_POSIX_V7_ILP32_OFF32_CFLAGS,

    _CS_POSIX_V7_ILP32_OFF32_LDFLAGS,

    _CS_POSIX_V7_ILP32_OFF32_LIBS,

    _CS_POSIX_V7_ILP32_OFF32_LINTFLAGS,

    _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS,

    _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS,

    _CS_POSIX_V7_ILP32_OFFBIG_LIBS,

    _CS_POSIX_V7_ILP32_OFFBIG_LINTFLAGS,

    _CS_POSIX_V7_LP64_OFF64_CFLAGS,

    _CS_POSIX_V7_LP64_OFF64_LDFLAGS,

    _CS_POSIX_V7_LP64_OFF64_LIBS,

    _CS_POSIX_V7_LP64_OFF64_LINTFLAGS,

    _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS,

    _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS,

    _CS_POSIX_V7_LPBIG_OFFBIG_LIBS,

    _CS_POSIX_V7_LPBIG_OFFBIG_LINTFLAGS,


    _CS_V6_ENV,

    _CS_V7_ENV

  };
# 613 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1)))
# 613 "/usr/include/unistd.h"
long pathconf (const char *__path, int __name);
# 617 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))long fpathconf(int __fd,int __name);


extern __attribute__((__nothrow__,__leaf__))long sysconf(int __name);
# 624 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))size_t confstr(int __name,char*__buf,size_t __len);
# 629 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))__pid_t getpid(void);


extern __attribute__((__nothrow__,__leaf__))__pid_t getppid(void);
# 637 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))__pid_t getpgrp(void);
# 647 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))__pid_t __getpgid(__pid_t __pid);

extern __attribute__((__nothrow__,__leaf__))__pid_t getpgid(__pid_t __pid);
# 656 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int setpgid(__pid_t __pid,__pid_t __pgid);
# 673 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int setpgrp(void);
# 690 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))__pid_t setsid(void);
# 694 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))__pid_t getsid(__pid_t __pid);
# 698 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))__uid_t getuid(void);


extern __attribute__((__nothrow__,__leaf__))__uid_t geteuid(void);


extern __attribute__((__nothrow__,__leaf__))__gid_t getgid(void);


extern __attribute__((__nothrow__,__leaf__))__gid_t getegid(void);
# 712 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int getgroups(int __size,__gid_t*__list);
# 723 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int setuid(__uid_t __uid);
# 728 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int setreuid(__uid_t __ruid,__uid_t __euid);
# 733 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int seteuid(__uid_t __uid);
# 740 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int setgid(__gid_t __gid);
# 745 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int setregid(__gid_t __rgid,__gid_t __egid);
# 750 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int setegid(__gid_t __gid);
# 779 "/usr/include/unistd.h"
extern __attribute__((__nothrow__))__pid_t fork(void);
# 793 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))char*ttyname(int __fd);
# 797 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           2)))
# 797 "/usr/include/unistd.h"
int ttyname_r (int __fd, char *__buf, size_t __buflen);
# 802 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int isatty(int __fd);
# 813 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1, 2)))
# 813 "/usr/include/unistd.h"
int link (const char *__from, const char *__to);
# 819 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (

                                                                           2, 4)))
# 819 "/usr/include/unistd.h"
int linkat (int __fromfd, const char *__from, int __tofd,
     const char *__to, int __flags);
# 826 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1, 2)))
# 826 "/usr/include/unistd.h"
int symlink (const char *__from, const char *__to);
# 832 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (

                                                                           1, 2)))
# 832 "/usr/include/unistd.h"
ssize_t readlink (const char *__restrict __path,
    char *__restrict __buf, size_t __len);
# 839 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                                                  1, 3)))
# 839 "/usr/include/unistd.h"
int symlinkat (const char *__from, int __tofd,
        const char *__to);


extern __attribute__((__nothrow__,__leaf__,__nonnull__ (

                                                                           2, 3)))
# 843 "/usr/include/unistd.h"
ssize_t readlinkat (int __fd, const char *__restrict __path,
      char *__restrict __buf, size_t __len);
# 849 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))int unlink(const char*__name);
# 853 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           2)))
# 853 "/usr/include/unistd.h"
int unlinkat (int __fd, const char *__name, int __flag);
# 858 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))int rmdir(const char*__path);
# 862 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))__pid_t tcgetpgrp(int __fd);


extern __attribute__((__nothrow__,__leaf__))int tcsetpgrp(int __fd,__pid_t __pgrp_id);
# 872 "/usr/include/unistd.h"
extern char *getlogin (void);
# 880 "/usr/include/unistd.h"
extern __attribute__((__nonnull__ ( 1)))int getlogin_r(char*__name,size_t __name_len);
# 59 "/usr/include/getopt.h"
extern char *optarg;
# 73 "/usr/include/getopt.h"
extern int optind;
# 78 "/usr/include/getopt.h"
extern int opterr;
# 82 "/usr/include/getopt.h"
extern int optopt;
# 152 "/usr/include/getopt.h"
extern __attribute__((__nothrow__,__leaf__))int getopt(int ___argc,char*const *___argv,const char*__shortopts);
# 161 "/usr/include/getopt.h"
  extern __attribute__((__nothrow__,__leaf__))int getopt(int ___argc,char*const *___argv,const char*__shortopts) __asm__("""__posix_getopt");
# 902 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))int gethostname(char*__name,size_t __len);
# 980 "/usr/include/unistd.h"
extern int fsync (int __fd);
# 994 "/usr/include/unistd.h"
extern long gethostid (void);


extern __attribute__((__nothrow__,__leaf__))void sync(void);
# 1018 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1)))
# 1018 "/usr/include/unistd.h"
int truncate (const char *__file, __off_t __length);
# 1040 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))int ftruncate(int __fd,__off_t __length);
# 1105 "/usr/include/unistd.h"
extern int lockf (int __fd, int __cmd, __off_t __len);
# 1136 "/usr/include/unistd.h"
extern int fdatasync (int __fildes);
# 1144 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1, 2)))
# 1144 "/usr/include/unistd.h"
char *crypt (const char *__key, const char *__salt);
# 1149 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))void encrypt(char*__libc_block,int __edflag);
# 1156 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                                       1, 2)))
# 1156 "/usr/include/unistd.h"
void swab (const void *__restrict __from, void *__restrict __to,
    ssize_t __n);
# 1165 "/usr/include/unistd.h"
extern __attribute__((__nothrow__,__leaf__))char*ctermid(char*__s);
# 522 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-sysdep.h"
typedef long long Cilk_time;
# 524 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-sysdep.h"
static inline Cilk_time Cilk_get_time(void)
{
     struct timeval tv;
     gettimeofday(&tv, 0);

     return ((Cilk_time)tv.tv_sec * 1000000LL+ (Cilk_time)tv.tv_usec);
}
# 532 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-sysdep.h"
static inline double Cilk_time_to_sec(Cilk_time t)
{
     return (double) t * 1.0E-6;
}
# 537 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-sysdep.h"
static inline Cilk_time Cilk_get_wall_time(void)
{
     return Cilk_get_time();
}
# 542 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-sysdep.h"
static inline double Cilk_wall_time_to_sec(Cilk_time t)
{
     return Cilk_time_to_sec(t);
}
# 44 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
static __attribute__((__unused__))const char*cilk_h_ident= "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/cilk.h $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $";
# 48 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
enum WorkerType { NORMAL_WORKER, DS_WORKER };


enum DSSTatus { DS_WAITING, DS_DONE };
# 27 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-conf.h"
static __attribute__((__unused__))const char*ident_cilk_conf_h= "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/cilk-conf.h $ $LastChangedBy: bradley $ $Rev: 1465 $ $Date: 2004-08-02 06:31:06 -0400 (Mon, 02 Aug 2004) $";
# 84 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
typedef struct ___sue18{
  int size;
  ptrdiff_t index;
  void( *inlet) ();
  int argsize;
  ptrdiff_t argindex;
}
# 84 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
CilkProcInfo;
# 95 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
struct cilk_alloca_header {
  struct cilk_alloca_header *next;
  size_t size;
};
# 103 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
typedef void( *HookT)(void);

typedef struct hook {
  HookT fn;
  struct hook *next;
}
# 105 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
HookList;
# 110 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
extern void Cilk_add_hook(HookList **listp, HookT fn);
extern void Cilk_run_hooks(HookList *list);
# 119 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
extern HookList *Cilk_init_global_hooks;
extern HookList *Cilk_init_per_worker_hooks;
# 135 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
typedef struct ___sue19{
  int entry;
  void *receiver;

  CilkProcInfo *sig;
  struct cilk_alloca_header *alloca_h;
# 149 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
}
# 135 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
CilkStackFrame;
# 154 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
typedef CilkStackFrame **CilkStack;
# 172 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
typedef struct ___sue20{
  volatile CilkStackFrame **head;volatile CilkStackFrame**tail;

  volatile CilkStackFrame **exception;
  CilkStack stack;
  char __dummy[64];
}
# 172 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
CilkClosureCache;
# 181 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
struct Cilk_im_descriptor {
  void *free_list;
  int length;
# 188 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
  int count;
};


struct Cilk_im_stats {
  int used;
  int nmalloc;
  int in_free_lists;


  int length[9];
  char __dummy[64];
};


typedef struct Closure_s Closure;
typedef struct Cilk_options_s Cilk_options;
# 210 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
typedef struct ___sue21{
# 217 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
  struct Cilk_im_stats *im_info;


  Cilk_options *options;
# 226 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
  const char *assertion_failed_msg;
  const char *stack_overflow_msg;


  int active_size;
  int pthread_stacksize;


  double dsratio;
# 241 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
  void *infofile;
# 257 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
  unsigned num_threads;
  unsigned num_steals;

  unsigned num_real_workers;
  unsigned num_ds_workers;
# 267 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
  struct ReadyDeque *deques;

  Cilk_time start_time;

  Closure *invoke_main;

  Closure *invoke_ds_main;


  HookList *Cilk_init_global_hooks;
  HookList *Cilk_init_per_worker_hooks;

}
# 210 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
CilkReadOnlyParams;
# 282 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
typedef struct BatchOp{

  void( *operation)(void**, size_t);
  void *args;
  size_t size;
  volatile int status;
  char padding[1024];

}
# 282 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
BatchOp;
# 294 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
typedef struct ___sue22{
  int nprocs;
  BatchOp*array;
}
# 294 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
work_array;
# 299 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
work_array ds_work_array;

typedef struct CilkGlobalState_s CilkGlobalState;

typedef struct ___sue23{

  CilkReadOnlyParams *Cilk_RO_params;
# 310 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
  CilkGlobalState *Cilk_global_state;

}
# 303 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
CilkContext;
# 316 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
typedef struct ___sue24{
  CilkClosureCache cache;
  int self;
  struct Cilk_im_descriptor im_descriptor [9];
  size_t stackdepth;
  Cilk_time last_cp_time;
  Cilk_time cp_hack;
  Cilk_time work_hack;
  Cilk_time user_work;
  Cilk_time user_critical_path;
  unsigned rand_next;
  int abort_flag;
  int barrier_direction;
  char __dummy[64];
  CilkContext *context;
# 335 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
  enum WorkerType type;
}
# 316 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
CilkWorkerState;
# 338 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
typedef struct ___sue25{

  CilkContext *context;
  int id;

}
# 338 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
CilkChildParams;
# 349 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
extern void Cilk_dprintf(CilkWorkerState *const ws, const char *fmt,...);
extern void Cilk_die_internal(CilkContext *const context, CilkWorkerState *const ws, const char *fmt,...);
extern void Cilk_unalloca_internal(CilkWorkerState *const ws,
       CilkStackFrame *f);
# 357 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
extern void *Cilk_internal_malloc(CilkWorkerState *const ws, size_t);
extern void Cilk_internal_free(CilkWorkerState *const ws, void *p, size_t size);
# 370 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
extern CilkContext *Cilk_init(int*argc, char**argv);
extern void Cilk_terminate(CilkContext *const context);





static inline Cilk_time Cilk_get_elapsed_time(CilkWorkerState *const ws)
{
  Cilk_time then= ws->last_cp_time;
  Cilk_time now= Cilk_get_time();;
# 384 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
  ws->last_cp_time = now;
  return now - then;
}
# 397 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
static inline int Cilk_internal_malloc_canonicalize(size_t size)
{
  if (size <= 16 && 16 >= 64) return 16;;
  if (size <= 32 && 32 >= 64) return 32;;
  if (size <= 64 && 64 >= 64) return 64;;
  if (size <= 128 && 128 >= 64) return 128;;
  if (size <= 256 && 256 >= 64) return 256;;
  if (size <= 512 && 512 >= 64) return 512;;
  if (size <= 1024 && 1024 >= 64) return 1024;;
  if (size <= 2048 && 2048 >= 64) return 2048;;
  if (size <= 4096 && 4096 >= 64) return 4096;;
  return -1;
}




static inline int Cilk_internal_malloc_size_to_bucket(size_t size)
{
  if (size <= 16 && 16 >= 64) return 0;;
  if (size <= 32 && 32 >= 64) return 1;;
  if (size <= 64 && 64 >= 64) return 2;;
  if (size <= 128 && 128 >= 64) return 3;;
  if (size <= 256 && 256 >= 64) return 4;;
  if (size <= 512 && 512 >= 64) return 5;;
  if (size <= 1024 && 1024 >= 64) return 6;;
  if (size <= 2048 && 2048 >= 64) return 7;;
  if (size <= 4096 && 4096 >= 64) return 8;;
  return -1;
}




static inline int Cilk_internal_malloc_bucket_to_size(int b)
{
  if (0 == b) return 16;;
  if (1 == b) return 32;;
  if (2 == b) return 64;;
  if (3 == b) return 128;;
  if (4 == b) return 256;;
  if (5 == b) return 512;;
  if (6 == b) return 1024;;
  if (7 == b) return 2048;;
  if (8 == b) return 4096;;
  return -1;
}
# 452 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
static inline void Cilk_fence(void)
{
  CILK_MB();
}





static inline void Cilk_membar_StoreStore(void)
{
  __asm__ volatile ( "" : : : "memory");
}





static inline void Cilk_membar_StoreLoad(void)
{
  CILK_MB();
}
# 479 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
extern int Cilk_sync(CilkWorkerState *const ws);
extern int Cilk_exception_handler(CilkWorkerState *const ws, void *, int);
extern void Cilk_set_result(CilkWorkerState *const ws,
       void *resultp, int size);
extern void Cilk_after_sync_slow_cp(CilkWorkerState *const ws,
        Cilk_time *work, Cilk_time *cp);
extern void Cilk_abort_standalone(CilkWorkerState *const ws);
extern void Cilk_abort_slow(CilkWorkerState *const ws);
extern void Cilk_event_new_thread(CilkWorkerState *const ws);
extern void Cilk_destroy_frame(CilkWorkerState *const ws,
          CilkStackFrame *f, size_t size);
# 2 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static __attribute__((__unused__))const char*ident_cilk_cilk2c_pre= "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/cilk-cilk2c-pre.h $ $LastChangedBy: bradley $ $Rev: 1465 $ $Date: 2004-08-02 06:31:06 -0400 (Mon, 02 Aug 2004) $";
# 31 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void *Cilk_internal_malloc_fast(CilkWorkerState *const ws,
        size_t size)
{
     int bucket;
     void *mem;
     struct Cilk_im_descriptor *d;;
# 40 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
     if (size > 4096)
   return Cilk_internal_malloc(ws, size);

     bucket = Cilk_internal_malloc_size_to_bucket(size);
     d = ws->im_descriptor + bucket;


     if ((mem= d->free_list)) {
   d->free_list = ((void **) mem)[0];
   d->count++;
# 55 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
     } else {
   mem = Cilk_internal_malloc(ws, size);
     }

     return mem;
}
# 62 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_internal_free_fast(CilkWorkerState *const ws,
            void *p, size_t size)
{
     int bucket;
     struct Cilk_im_descriptor *d;

     if (size > 4096) {
   Cilk_internal_free(ws, p, size);
   return;
     }

     bucket = Cilk_internal_malloc_size_to_bucket(size);
     d = ws->im_descriptor + bucket;

     if (d->count <= 0)
   Cilk_internal_free(ws, p, size);
     else {
   ((void **) p)[0] = d->free_list;
   d->free_list = p;
   d->count--;;
# 87 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
     }
}
# 90 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_destroy_frame_fast(CilkWorkerState *const ws,
            CilkStackFrame *f, size_t size)
{
     { if (f->alloca_h) Cilk_unalloca_internal(ws, f); };
# 99 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
     Cilk_internal_free_fast(ws, f, size);
}
# 102 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void *Cilk_create_frame(CilkWorkerState *const ws,
       size_t size, CilkProcInfo *sig)
{
     CilkStackFrame *f= Cilk_internal_malloc_fast(ws, size);
     f->sig = sig;
     f->alloca_h = (struct cilk_alloca_header *) 0;;


     return (void *) f;
}
# 113 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_push_frame( __attribute__((__unused__))CilkWorkerState*const ws,
           __attribute__((__unused__))CilkStackFrame*frame)
{;;;


     Cilk_membar_StoreStore();
}
# 121 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void *Cilk_cilk2c_init_frame(CilkWorkerState *const ws,
            size_t s, CilkProcInfo *signat)
{
     volatile CilkStackFrame **t;
     void *f;

     f = Cilk_create_frame(ws, s, signat);
     t = ws->cache.tail;;


     *t = (CilkStackFrame *) f;
     Cilk_membar_StoreStore();
     ws->cache.tail = t + 1;
     return f;
}




static inline int Cilk_cilk2c_pop_check(CilkWorkerState *const ws)
{
     volatile CilkStackFrame **t;
     t = ws->cache.tail;
     Cilk_membar_StoreLoad();
     return (ws->cache.exception>= t);
}
# 148 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_pop(CilkWorkerState *const ws)
{
     --ws->cache.tail;
}
# 153 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_event_new_thread_maybe(
                                   __attribute__((__unused__))CilkWorkerState*const ws)
{;;


}
# 160 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_start_thread_slow( __attribute__((__unused__))CilkWorkerState*const ws,
           __attribute__((__unused__))CilkStackFrame*frame)
{;;;
# 166 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
}
# 168 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_before_return_fast(CilkWorkerState *const ws,
        CilkStackFrame *frame,
            size_t size)
{;;


     Cilk_destroy_frame_fast(ws, (CilkStackFrame *) frame, size);
     --ws->cache.tail;
}
# 178 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_before_return_slow(CilkWorkerState *const ws,
        CilkStackFrame *frame,
            size_t size)
{
     Cilk_cilk2c_before_return_fast(ws, frame, size);
}
# 345 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_start_thread_fast_cp(
     __attribute__((__unused__))CilkWorkerState*const ws, __attribute__((__unused__))CilkStackFrame*frame)
{;;


}
# 351 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_start_thread_slow_cp(
     __attribute__((__unused__))CilkWorkerState*const ws, __attribute__((__unused__))CilkStackFrame*frame)
{;;


}
# 357 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_at_thread_boundary_slow_cp(
     __attribute__((__unused__))CilkWorkerState*const ws, __attribute__((__unused__))CilkStackFrame*frame)
{;;


}
# 363 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_before_spawn_fast_cp(
     __attribute__((__unused__))CilkWorkerState*const ws, __attribute__((__unused__))CilkStackFrame*frame)
{;;


}
# 369 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_before_spawn_slow_cp(
     __attribute__((__unused__))CilkWorkerState*const ws, __attribute__((__unused__))CilkStackFrame*frame)
{;;


}
# 375 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_after_spawn_fast_cp(
     __attribute__((__unused__))CilkWorkerState*const ws, __attribute__((__unused__))CilkStackFrame*frame)
{;;


}
# 381 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_after_spawn_slow_cp(
     __attribute__((__unused__))CilkWorkerState*const ws, __attribute__((__unused__))CilkStackFrame*frame)
{;;


}
# 387 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_at_sync_fast_cp(
     __attribute__((__unused__))CilkWorkerState*const ws, __attribute__((__unused__))CilkStackFrame*frame)
{;;


}
# 393 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_before_sync_slow_cp(
     __attribute__((__unused__))CilkWorkerState*const ws, __attribute__((__unused__))CilkStackFrame*frame)
{;;


}
# 399 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_after_sync_slow_cp(
     __attribute__((__unused__))CilkWorkerState*const ws, __attribute__((__unused__))CilkStackFrame*frame)
{;;


}
# 405 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_before_return_fast_cp(
     __attribute__((__unused__))CilkWorkerState*const ws, __attribute__((__unused__))CilkStackFrame*frame)
{;;


}
# 411 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk-cilk2c-pre.h"
static inline void Cilk_cilk2c_before_return_slow_cp(
     __attribute__((__unused__))CilkWorkerState*const ws, __attribute__((__unused__))CilkStackFrame*frame)
{;;


}
# 537 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
extern int Cilk_flags_are_wrong_NODEBUG_NOTIMING_NOSTATS_please_recompile;
static int *Cilk_check_flags_at_link_time= &Cilk_flags_are_wrong_NODEBUG_NOTIMING_NOSTATS_please_recompile;

static __attribute__((__unused__))int Cilk_check_flags_at_link_time_hack(void);
# 541 "/home/bss45/research/parallel_data_structures/parallelDS/split/runtime/cilk.h"
static int Cilk_check_flags_at_link_time_hack(void) {
  return *Cilk_check_flags_at_link_time;
}


void Cilk_start(CilkContext *const context,
  void( *main)(CilkWorkerState *const ws, void *args),
  void *args,
  int return_size);

void Cilk_free(void *);
void *Cilk_malloc_fixed(size_t);
# 45 "/usr/include/stdio.h"
struct _IO_FILE;
# 49 "/usr/include/stdio.h"
typedef struct _IO_FILE FILE;
# 65 "/usr/include/stdio.h"
typedef struct _IO_FILE __FILE;
# 83 "/usr/include/wchar.h"
typedef struct ___sue27
{
  int __count;
  union ___sue26
  {

    unsigned __wch;
# 93 "/usr/include/wchar.h"
    char __wchb[4];
  }
# 86 "/usr/include/wchar.h"
  __value;
# 95 "/usr/include/wchar.h"
}
# 83 "/usr/include/wchar.h"
__mbstate_t;
# 22 "/usr/include/_G_config.h"
typedef struct ___sue28
{
  __off_t __pos;
  __mbstate_t __state;
}
# 22 "/usr/include/_G_config.h"
_G_fpos_t;
# 27 "/usr/include/_G_config.h"
typedef struct ___sue29
{
  __off64_t __pos;
  __mbstate_t __state;
}
# 27 "/usr/include/_G_config.h"
_G_fpos64_t;
# 53 "/usr/include/_G_config.h"
typedef __attribute__((__mode__ ( __HI__)))int _G_int16_t;
typedef __attribute__((__mode__ ( __SI__)))int _G_int32_t;
typedef __attribute__((__mode__ ( __HI__)))unsigned _G_uint16_t;
typedef __attribute__((__mode__ ( __SI__)))unsigned _G_uint32_t;
# 40 "/usr/lib/gcc/x86_64-linux-gnu/4.7/include/stdarg.h"
typedef __builtin_va_list __gnuc_va_list;
# 172 "/usr/include/libio.h"
struct _IO_jump_t; struct _IO_FILE;
# 182 "/usr/include/libio.h"
typedef void _IO_lock_t;
# 188 "/usr/include/libio.h"
struct _IO_marker {
  struct _IO_marker *_next;
  struct _IO_FILE *_sbuf;
# 194 "/usr/include/libio.h"
  int _pos;
# 205 "/usr/include/libio.h"
};


enum __codecvt_result
{
  __codecvt_ok,
  __codecvt_partial,
  __codecvt_error,
  __codecvt_noconv
};
# 273 "/usr/include/libio.h"
struct _IO_FILE {
  int _flags;
# 279 "/usr/include/libio.h"
  char*_IO_read_ptr;
  char*_IO_read_end;
  char*_IO_read_base;
  char*_IO_write_base;
  char*_IO_write_ptr;
  char*_IO_write_end;
  char*_IO_buf_base;
  char*_IO_buf_end;

  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;
# 300 "/usr/include/libio.h"
  int _flags2;

  __off_t _old_offset;
# 306 "/usr/include/libio.h"
  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];
# 312 "/usr/include/libio.h"
  _IO_lock_t *_lock;
# 321 "/usr/include/libio.h"
  __off64_t _offset;
# 330 "/usr/include/libio.h"
  void *__pad1;
  void *__pad2;
  void *__pad3;
  void *__pad4;
  size_t __pad5;

  int _mode;

  char _unused2[15 * sizeof( int) - 4 * sizeof( void *) - sizeof( size_t)];

};


typedef struct _IO_FILE _IO_FILE;


struct _IO_FILE_plus;

extern struct _IO_FILE_plus _IO_2_1_stdin_;
extern struct _IO_FILE_plus _IO_2_1_stdout_;
extern struct _IO_FILE_plus _IO_2_1_stderr_;
# 366 "/usr/include/libio.h"
typedef __ssize_t __io_read_fn (void *__cookie, char *__buf, size_t __nbytes);
# 374 "/usr/include/libio.h"
typedef __ssize_t __io_write_fn (void *__cookie, const char *__buf,
     size_t __n);
# 383 "/usr/include/libio.h"
typedef int __io_seek_fn (void *__cookie, __off64_t *__pos, int __w);


typedef int __io_close_fn (void *__cookie);
# 418 "/usr/include/libio.h"
extern int __underflow (_IO_FILE *);
extern int __uflow (_IO_FILE *);
extern int __overflow (_IO_FILE *, int);
# 462 "/usr/include/libio.h"
extern int _IO_getc (_IO_FILE *__fp);
extern int _IO_putc (int __c, _IO_FILE *__fp);
extern __attribute__((__nothrow__,__leaf__))int _IO_feof(_IO_FILE*__fp);
extern __attribute__((__nothrow__,__leaf__))int _IO_ferror(_IO_FILE*__fp);

extern int _IO_peekc_locked (_IO_FILE *__fp);
# 473 "/usr/include/libio.h"
extern __attribute__((__nothrow__,__leaf__))void _IO_flockfile(_IO_FILE*);
extern __attribute__((__nothrow__,__leaf__))void _IO_funlockfile(_IO_FILE*);
extern __attribute__((__nothrow__,__leaf__))int _IO_ftrylockfile(_IO_FILE*);
# 492 "/usr/include/libio.h"
extern int _IO_vfscanf (_IO_FILE *__restrict , const char *__restrict ,
   __gnuc_va_list, int *__restrict );
extern int _IO_vfprintf (_IO_FILE *__restrict ,const char *__restrict ,
    __gnuc_va_list);
extern __ssize_t _IO_padn (_IO_FILE *, int, __ssize_t);
extern size_t _IO_sgetn (_IO_FILE *, void *, size_t);

extern __off64_t _IO_seekoff (_IO_FILE *, __off64_t, int, int);
extern __off64_t _IO_seekpos (_IO_FILE *, __off64_t, int);

extern __attribute__((__nothrow__,__leaf__))void _IO_free_backup_area(_IO_FILE*);
# 80 "/usr/include/stdio.h"
typedef __gnuc_va_list va_list;
# 111 "/usr/include/stdio.h"
typedef _G_fpos_t fpos_t;
# 169 "/usr/include/stdio.h"
extern struct _IO_FILE *stdin;
extern struct _IO_FILE *stdout;
extern struct _IO_FILE *stderr;
# 179 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__))int remove(const char*__filename);

extern __attribute__((__nothrow__,__leaf__))int rename(const char*__old,const char*__new);
# 186 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__))int renameat(int __oldfd,const char*__old,int __newfd,
       const char *__new);
# 196 "/usr/include/stdio.h"
extern FILE *tmpfile (void);
# 210 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__))char*tmpnam(char*__s);
# 228 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__,__malloc__))char*tempnam(const char*__dir,const char*__pfx);
# 238 "/usr/include/stdio.h"
extern int fclose (FILE *__stream);
# 243 "/usr/include/stdio.h"
extern int fflush (FILE *__stream);
# 273 "/usr/include/stdio.h"
extern FILE *fopen (const char *__restrict __filename,
      const char *__restrict __modes);
# 279 "/usr/include/stdio.h"
extern FILE *freopen (const char *__restrict __filename,
        const char *__restrict __modes,
        FILE *__restrict __stream);
# 307 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__))FILE*fdopen(int __fd,const char*__modes);
# 320 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__))FILE*fmemopen(void*__s,size_t __len,const char*__modes);
# 326 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__))FILE*open_memstream(char**__bufloc,size_t*__sizeloc);
# 333 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__))void setbuf(FILE*__restrict __stream,char*__restrict __buf);
# 337 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__))int setvbuf(FILE*__restrict __stream,char*__restrict __buf,
      int __modes, size_t __n);
# 357 "/usr/include/stdio.h"
extern int fprintf (FILE *__restrict __stream,
      const char *__restrict __format,...);
# 363 "/usr/include/stdio.h"
extern int printf (const char *__restrict __format,...);

extern __attribute__((__nothrow__))int sprintf(char*__restrict __s,
      const char *__restrict __format,...);
# 372 "/usr/include/stdio.h"
extern int vfprintf (FILE *__restrict __s, const char *__restrict __format,
       __gnuc_va_list __arg);
# 378 "/usr/include/stdio.h"
extern int vprintf (const char *__restrict __format, __gnuc_va_list __arg);

extern __attribute__((__nothrow__))int vsprintf(char*__restrict __s,const char*__restrict __format,
       __gnuc_va_list __arg);
# 387 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__format__ (

                                                               __printf__, 3, 4)))
# 387 "/usr/include/stdio.h"
int snprintf (char *__restrict __s, size_t __maxlen,
       const char *__restrict __format,...);


extern __attribute__((__nothrow__,__format__ (

                                                               __printf__, 3, 0)))
# 391 "/usr/include/stdio.h"
int vsnprintf (char *__restrict __s, size_t __maxlen,
        const char *__restrict __format, __gnuc_va_list __arg);
# 418 "/usr/include/stdio.h"
extern __attribute__((__format__ (

                                 __printf__, 2, 0)))
# 418 "/usr/include/stdio.h"
int vdprintf (int __fd, const char *__restrict __fmt,
       __gnuc_va_list __arg);

extern __attribute__((__format__ (
                                 __printf__, 2, 3)))
# 421 "/usr/include/stdio.h"
int dprintf (int __fd, const char *__restrict __fmt,...);
# 431 "/usr/include/stdio.h"
extern int fscanf (FILE *__restrict __stream,
     const char *__restrict __format,...);
# 437 "/usr/include/stdio.h"
extern int scanf (const char *__restrict __format,...);

extern __attribute__((__nothrow__,__leaf__))int sscanf(const char*__restrict __s,
     const char *__restrict __format,...);
# 449 "/usr/include/stdio.h"
extern int fscanf (FILE *__restrict __stream, const char *__restrict __format,...) __asm__( """__isoc99_fscanf");


extern int scanf (const char *__restrict __format,...) __asm__( """__isoc99_scanf");

extern __attribute__((__nothrow__,__leaf__))int sscanf(const char*__restrict __s,const char*__restrict __format,...) __asm__("""__isoc99_sscanf");
# 477 "/usr/include/stdio.h"
extern __attribute__((__format__ (

                                 __scanf__, 2, 0)))
# 477 "/usr/include/stdio.h"
int vfscanf (FILE *__restrict __s, const char *__restrict __format,
      __gnuc_va_list __arg);
# 485 "/usr/include/stdio.h"
extern __attribute__((__format__ (
                                 __scanf__, 1, 0)))
# 485 "/usr/include/stdio.h"
int vscanf (const char *__restrict __format, __gnuc_va_list __arg);
# 489 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__,__format__ (

                                                                          __scanf__, 2, 0)))
# 489 "/usr/include/stdio.h"
int vsscanf (const char *__restrict __s,
      const char *__restrict __format, __gnuc_va_list __arg);
# 500 "/usr/include/stdio.h"
extern __attribute__((__format__ (
# 504 "/usr/include/stdio.h"
                                 __scanf__, 2, 0)))
# 500 "/usr/include/stdio.h"
int vfscanf (FILE *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __asm__( """__isoc99_vfscanf");
# 505 "/usr/include/stdio.h"
extern __attribute__((__format__ (

                                 __scanf__, 1, 0)))
# 505 "/usr/include/stdio.h"
int vscanf (const char *__restrict __format, __gnuc_va_list __arg) __asm__( """__isoc99_vscanf");


extern __attribute__((__nothrow__,__leaf__,__format__ (
# 512 "/usr/include/stdio.h"
                                 __scanf__, 2, 0)))
# 508 "/usr/include/stdio.h"
int vsscanf (const char *__restrict __s, const char *__restrict __format, __gnuc_va_list __arg) __asm__( """__isoc99_vsscanf");
# 537 "/usr/include/stdio.h"
extern int fgetc (FILE *__stream);
extern int getc (FILE *__stream);
# 544 "/usr/include/stdio.h"
extern int getchar (void);
# 556 "/usr/include/stdio.h"
extern int getc_unlocked (FILE *__stream);
extern int getchar_unlocked (void);
# 579 "/usr/include/stdio.h"
extern int fputc (int __c, FILE *__stream);
extern int putc (int __c, FILE *__stream);
# 586 "/usr/include/stdio.h"
extern int putchar (int __c);
# 608 "/usr/include/stdio.h"
extern int putc_unlocked (int __c, FILE *__stream);
extern int putchar_unlocked (int __c);
# 628 "/usr/include/stdio.h"
extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream);
# 636 "/usr/include/stdio.h"
extern char *gets (char *__s);
# 662 "/usr/include/stdio.h"
extern __ssize_t __getdelim (char **__restrict __lineptr,
          size_t *__restrict __n, int __delimiter,
          FILE *__restrict __stream);
extern __ssize_t getdelim (char **__restrict __lineptr,
        size_t *__restrict __n, int __delimiter,
        FILE *__restrict __stream);
# 675 "/usr/include/stdio.h"
extern __ssize_t getline (char **__restrict __lineptr,
       size_t *__restrict __n,
       FILE *__restrict __stream);
# 686 "/usr/include/stdio.h"
extern int fputs (const char *__restrict __s, FILE *__restrict __stream);
# 692 "/usr/include/stdio.h"
extern int puts (const char *__s);
# 699 "/usr/include/stdio.h"
extern int ungetc (int __c, FILE *__stream);
# 706 "/usr/include/stdio.h"
extern size_t fread (void *__restrict __ptr, size_t __size,
       size_t __n, FILE *__restrict __stream);
# 712 "/usr/include/stdio.h"
extern size_t fwrite (const void *__restrict __ptr, size_t __size,
        size_t __n, FILE *__restrict __s);
# 746 "/usr/include/stdio.h"
extern int fseek (FILE *__stream, long __off, int __whence);
# 751 "/usr/include/stdio.h"
extern long ftell (FILE *__stream);
# 756 "/usr/include/stdio.h"
extern void rewind (FILE *__stream);
# 770 "/usr/include/stdio.h"
extern int fseeko (FILE *__stream, __off_t __off, int __whence);
# 775 "/usr/include/stdio.h"
extern __off_t ftello (FILE *__stream);
# 795 "/usr/include/stdio.h"
extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos);
# 800 "/usr/include/stdio.h"
extern int fsetpos (FILE *__stream, const fpos_t *__pos);
# 823 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__))void clearerr(FILE*__stream);

extern __attribute__((__nothrow__,__leaf__))int feof(FILE*__stream);

extern __attribute__((__nothrow__,__leaf__))int ferror(FILE*__stream);
# 843 "/usr/include/stdio.h"
extern void perror (const char *__s);
# 855 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__))int fileno(FILE*__stream);
# 870 "/usr/include/stdio.h"
extern FILE *popen (const char *__command, const char *__modes);
# 876 "/usr/include/stdio.h"
extern int pclose (FILE *__stream);
# 882 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__))char*ctermid(char*__s);
# 888 "/usr/include/stdio.h"
extern char *cuserid (char *__s);
# 910 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__))void flockfile(FILE*__stream);
# 914 "/usr/include/stdio.h"
extern __attribute__((__nothrow__,__leaf__))int ftrylockfile(FILE*__stream);


extern __attribute__((__nothrow__,__leaf__))void funlockfile(FILE*__stream);
# 98 "/usr/include/stdlib.h"
typedef struct ___sue30
  {
    int quot;
    int rem;
  }
# 98 "/usr/include/stdlib.h"
div_t;
# 106 "/usr/include/stdlib.h"
typedef struct ___sue31
  {
    long quot;
    long rem;
  }
# 106 "/usr/include/stdlib.h"
ldiv_t;
# 118 "/usr/include/stdlib.h"
              typedef struct ___sue32
  {
    long long quot;
    long long rem;
  }
# 118 "/usr/include/stdlib.h"
              lldiv_t;
# 140 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))size_t __ctype_get_mb_cur_max(void);
# 145 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__pure__,__nonnull__ (
                                                                                                      1)))
# 145 "/usr/include/stdlib.h"
double atof (const char *__nptr);


extern __attribute__((__nothrow__,__leaf__,__pure__,__nonnull__ (
                                                                                                      1)))
# 148 "/usr/include/stdlib.h"
int atoi (const char *__nptr);


extern __attribute__((__nothrow__,__leaf__,__pure__,__nonnull__ (
                                                                                                      1)))
# 151 "/usr/include/stdlib.h"
long atol (const char *__nptr);
# 158 "/usr/include/stdlib.h"
              extern __attribute__((__nothrow__,__leaf__,__pure__,__nonnull__ (
                                                                                                      1)))
# 158 "/usr/include/stdlib.h"
              long long atoll (const char *__nptr);
# 165 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (

                                                                           1)))
# 165 "/usr/include/stdlib.h"
double strtod (const char *__restrict __nptr,
        char **__restrict __endptr);
# 173 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                                                         1)))
# 173 "/usr/include/stdlib.h"
float strtof (const char *__restrict __nptr,
       char **__restrict __endptr);

extern __attribute__((__nothrow__,__leaf__,__nonnull__ (

                                                                           1)))
# 176 "/usr/include/stdlib.h"
long double strtold (const char *__restrict __nptr,
       char **__restrict __endptr);
# 184 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (

                                                                           1)))
# 184 "/usr/include/stdlib.h"
long strtol (const char *__restrict __nptr,
   char **__restrict __endptr, int __base);


extern __attribute__((__nothrow__,__leaf__,__nonnull__ (

                                                                           1)))
# 188 "/usr/include/stdlib.h"
unsigned long strtoul (const char *__restrict __nptr,
      char **__restrict __endptr, int __base);
# 210 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (

                                                                           1)))
# 210 "/usr/include/stdlib.h"
long long strtoll (const char *__restrict __nptr,
         char **__restrict __endptr, int __base);
# 215 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (

                                                                           1)))
# 215 "/usr/include/stdlib.h"
unsigned long long strtoull (const char *__restrict __nptr,
     char **__restrict __endptr, int __base);
# 311 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))char*l64a(long __n);


extern __attribute__((__nothrow__,__leaf__,__pure__,__nonnull__ (
                                                                                                      1)))
# 314 "/usr/include/stdlib.h"
long a64l (const char *__s);
# 327 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))long random(void);


extern __attribute__((__nothrow__,__leaf__))void srandom(unsigned __seed);
# 336 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                                            2)))
# 336 "/usr/include/stdlib.h"
char *initstate (unsigned __seed, char *__statebuf,
   size_t __statelen);
# 341 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))char*setstate(char*__statebuf);
# 380 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))int rand(void);

extern __attribute__((__nothrow__,__leaf__))void srand(unsigned __seed);
# 387 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))int rand_r(unsigned*__seed);
# 395 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))double drand48(void);
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))double erand48(unsigned short*__xsubi);


extern __attribute__((__nothrow__,__leaf__))long lrand48(void);
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1)))
# 400 "/usr/include/stdlib.h"
long nrand48 (unsigned short *__xsubi);
# 404 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))long mrand48(void);
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1)))
# 405 "/usr/include/stdlib.h"
long jrand48 (unsigned short *__xsubi);
# 409 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))void srand48(long __seedval);
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1)))
# 410 "/usr/include/stdlib.h"
unsigned short *seed48 (unsigned short *__seed16v);

extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))void lcong48(unsigned short*__param);
# 471 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__malloc__))void*malloc(size_t __size);

extern __attribute__((__nothrow__,__leaf__,__malloc__))void*calloc(size_t __nmemb,size_t __size);
# 485 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__warn_unused_result__))void*realloc(void*__ptr,size_t __size);


extern __attribute__((__nothrow__,__leaf__))void free(void*__ptr);
# 508 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1)))
# 508 "/usr/include/stdlib.h"
int posix_memalign (void **__memptr, size_t __alignment, size_t __size);
# 514 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__noreturn__))void abort(void);
# 518 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))int atexit(void(*__func)(void));
# 544 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__noreturn__))void exit(int __status);
# 560 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__noreturn__))void _Exit(int __status);
# 567 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))char*getenv(const char*__name);
# 572 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           1)))
# 572 "/usr/include/stdlib.h"
char *__secure_getenv (const char *__name);
# 579 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))int putenv(char*__string);
# 585 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (
                                                                           2)))
# 585 "/usr/include/stdlib.h"
int setenv (const char *__name, const char *__value, int __replace);
# 589 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))int unsetenv(const char*__name);
# 620 "/usr/include/stdlib.h"
extern __attribute__((__nonnull__ ( 1)))int mkstemp(char*__template);
# 663 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))char*mkdtemp(char*__template);
# 717 "/usr/include/stdlib.h"
extern int system (const char *__command);
# 734 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))char*realpath(const char*__restrict __name,
         char *__restrict __resolved);
# 742 "/usr/include/stdlib.h"
typedef int( *__compar_fn_t) (const void *, const void *);
# 755 "/usr/include/stdlib.h"
extern __attribute__((__nonnull__ (

                                  1, 2, 5)))
# 755 "/usr/include/stdlib.h"
void *bsearch (const void *__key, const void *__base,
        size_t __nmemb, size_t __size, __compar_fn_t __compar);
# 761 "/usr/include/stdlib.h"
extern __attribute__((__nonnull__ (
                                                          1, 4)))
# 761 "/usr/include/stdlib.h"
void qsort (void *__base, size_t __nmemb, size_t __size,
     __compar_fn_t __compar);
# 771 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,const))int abs(int __x);
extern __attribute__((__nothrow__,__leaf__,const))long labs(long __x);
# 776 "/usr/include/stdlib.h"
              extern __attribute__((__nothrow__,__leaf__,const))long long llabs(long long __x);
# 785 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,const))div_t div(int __numer,int __denom);

extern __attribute__((__nothrow__,__leaf__,const))ldiv_t ldiv(long __numer,long __denom);
# 793 "/usr/include/stdlib.h"
              extern __attribute__((__nothrow__,__leaf__,const))lldiv_t lldiv(long long __numer,
        long long __denom);
# 860 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))int mblen(const char*__s,size_t __n);


extern __attribute__((__nothrow__,__leaf__))int mbtowc(wchar_t*__restrict __pwc,
     const char *__restrict __s, size_t __n);


extern __attribute__((__nothrow__,__leaf__))int wctomb(char*__s,wchar_t __wchar);
# 871 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))size_t mbstowcs(wchar_t*__restrict __pwcs,
   const char *__restrict __s, size_t __n);

extern __attribute__((__nothrow__,__leaf__))size_t wcstombs(char*__restrict __s,
   const wchar_t *__restrict __pwcs, size_t __n);
# 896 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ (


                                                                           1, 2, 3)))
# 896 "/usr/include/stdlib.h"
int getsubopt (char **__restrict __optionp,
        char *const *__restrict __tokens,
        char **__restrict __valuep);
# 905 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__,__nonnull__ ( 1)))void setkey(const char*__key);
# 913 "/usr/include/stdlib.h"
extern int posix_openpt (int __oflag);
# 921 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))int grantpt(int __fd);
# 925 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))int unlockpt(int __fd);
# 930 "/usr/include/stdlib.h"
extern __attribute__((__nothrow__,__leaf__))char*ptsname(int __fd);
# 59 "/usr/include/getopt.h"
extern char *optarg;
# 73 "/usr/include/getopt.h"
extern int optind;
# 78 "/usr/include/getopt.h"
extern int opterr;
# 82 "/usr/include/getopt.h"
extern int optopt;
# 106 "/usr/include/getopt.h"
struct option
{
  const char *name;


  int has_arg;
  int *flag;
  int val;
};
# 152 "/usr/include/getopt.h"
extern __attribute__((__nothrow__,__leaf__))int getopt(int ___argc,char*const *___argv,const char*__shortopts);
# 175 "/usr/include/getopt.h"
extern __attribute__((__nothrow__,__leaf__))int getopt_long(int ___argc,char*const *___argv,
   const char *__shortopts,
          const struct option *__longopts, int *__longind);

extern __attribute__((__nothrow__,__leaf__))int getopt_long_only(int ___argc,char*const *___argv,
        const char *__shortopts,
               const struct option *__longopts, int *__longind);
# 114 "/usr/include/strings.h"
extern __attribute__((__nothrow__,__leaf__,__pure__))int strcasecmp(const char*__s1,const char*__s2);
# 118 "/usr/include/strings.h"
extern __attribute__((__nothrow__,__leaf__,__pure__))int strncasecmp(const char*__s1,const char*__s2,size_t __n);
# 28 "/usr/include/xlocale.h"
typedef struct __locale_struct
{

  struct __locale_data *__locales[13];


  const unsigned short *__ctype_b;
  const int *__ctype_tolower;
  const int *__ctype_toupper;


  const char *__names[13];
} *
# 28 "/usr/include/xlocale.h"
__locale_t;
# 43 "/usr/include/xlocale.h"
typedef __locale_t locale_t;
# 129 "/usr/include/strings.h"
extern __attribute__((__nothrow__,__leaf__,__pure__,__nonnull__ (

                                                                                                      1, 2, 3)))
# 129 "/usr/include/strings.h"
int strcasecmp_l (const char *__s1, const char *__s2,
    __locale_t __loc);


extern __attribute__((__nothrow__,__leaf__,__pure__,__nonnull__ (

                                                                                                      1, 2, 4)))
# 133 "/usr/include/strings.h"
int strncasecmp_l (const char *__s1, const char *__s2,
     size_t __n, __locale_t __loc);
# 14 "./btree.h"
typedef enum ___sue33{false,true}bool;

typedef struct ___sue34{
        void *key;
        void *val;
}
# 16 "./btree.h"
bt_key_val;
# 21 "./btree.h"
typedef struct bt_node {
 struct bt_node *next;
 bool leaf;
        unsigned nr_active;
 unsigned level;
        bt_key_val **key_vals;
        struct bt_node **children;
}
# 21 "./btree.h"
bt_node;
# 30 "./btree.h"
typedef struct ___sue35{
 unsigned order;
 bt_node *root;
 unsigned( *value)(void *key);
        unsigned( *key_size)(void *key);
        unsigned( *data_size)(void *data);
 void( *print_key)(void *key);
}
# 30 "./btree.h"
btree;
# 39 "./btree.h"
extern btree *btree_create (unsigned order);
extern int btree_insert_key(btree *btree, bt_key_val *key_val);
extern int btree_delete_key(btree *btree, bt_node *subtree, void *key);
extern bt_key_val *btree_search (btree *btree, void *key);
extern void btree_destroy(btree *btree);
extern void *btree_get_max_key (btree *btree);
extern void *btree_get_min_key (btree *btree);
# 12 "btree.cilk"
static int recursive_flag= 1;
static int parallel_flag= 0;

struct timeval begin;struct timeval end;
double time_spent;

typedef enum ___sue36{left=-1,right=1}position_t;

typedef struct ___sue37{
 bt_node *node;
 unsigned index;
}
# 20 "btree.cilk"
node_pos;
# 25 "btree.cilk"
static void print_single_node(btree *btree, bt_node *node);
static bt_node *allocate_btree_node (unsigned order);
static int free_btree_node (bt_node *node);

static node_pos get_btree_node(btree *btree, void *key);

static int delete_key_from_node(btree *btree, node_pos *node_pos);
static bt_node *merge_nodes (btree *btree, bt_node *n1, bt_key_val *kv, bt_node *n2);
static void move_key(btree *btree, bt_node *node, unsigned index, position_t pos);
static node_pos get_max_key_pos(btree *btree, bt_node *subtree);
static node_pos get_min_key_pos(btree *btree, bt_node *subtree);
static bt_node *merge_siblings (btree *btree, bt_node *parent, unsigned index,
     position_t pos);
static void copy_key_val(btree *btree, bt_key_val *src, bt_key_val *dst);
# 45 "btree.cilk"
btree *btree_create (unsigned order) {
 btree *btree;
 btree = malloc(sizeof((*btree)));
 btree->order = order;
 btree->root = allocate_btree_node(order);
 btree->root->leaf = true;
 btree->root->nr_active = 0;
 btree->root->next = ((void *)0);
 btree->root->level = 0;
 return btree;
}
# 63 "btree.cilk"
static bt_node *allocate_btree_node (unsigned order) {
        bt_node *node;


        node = (bt_node *)malloc(sizeof(bt_node));


        node->nr_active = 0;


        node->key_vals = (bt_key_val **)malloc(2*order*sizeof(bt_key_val*) - 1);


        node->children = (bt_node **)malloc(2*order*sizeof(bt_node*));


 node->leaf = true;


 node->level = 0;


 node->next = ((void *)0);

        return node;
}
# 96 "btree.cilk"
static int free_btree_node (bt_node *node) {

        free(node->children);
        free(node->key_vals);
        free(node);

        return 0;
}
# 113 "btree.cilk"
static void btree_split_child(btree *btree, bt_node *parent,
    unsigned index,
    bt_node *child) {
 int i= 0;
 unsigned order= btree->order;

 bt_node *new_child= allocate_btree_node(btree->order);
 new_child->leaf = child->leaf;
 new_child->level = child->level;
 new_child->nr_active = btree->order - 1;


 for (i=0; i<order-1; i++) {
  new_child->key_vals[i] = child->key_vals[i + order];
  if (!child->leaf) {
   new_child->children[i] =
    child->children[i + order];
  }
 }


 if (!child->leaf) {
  new_child->children[i] =
  child->children[i + order];
 }

 child->nr_active = order - 1;

 for (i= parent->nr_active + 1; i> index + 1; i--) {
  parent->children[i] = parent->children[i - 1];
 }
 parent->children[index + 1] = new_child;

 for (i= parent->nr_active; i> index; i--) {
  parent->key_vals[i] = parent->key_vals[i - 1];
 }

 parent->key_vals[index] = child->key_vals[order - 1];
 parent->nr_active++;
}
# 162 "btree.cilk"
static void btree_insert_nonfull (btree *btree, bt_node *parent_node,
    bt_key_val *key_val) {

 unsigned key= btree->value(key_val->key);
 int i;
 bt_node *child;
 bt_node *node= parent_node;

insert: i = node->nr_active - 1;
 if (node->leaf) {
  while (i>= 0 && key < btree->value(node->key_vals[i]->key)) {
   node->key_vals[i + 1] = node->key_vals[i];
   i--;
  }
  node->key_vals[i + 1] = key_val;
  node->nr_active++;
 } else {
  while (i >= 0 && key < btree->value(node->key_vals[i]->key)) {
   i--;
  }
  i++;
  child = node->children[i];

  if (child->nr_active== 2*btree->order - 1) {
   btree_split_child(btree,node,i,child);
   if (btree->value(key_val->key)>
    btree->value(node->key_vals[i]->key)) {
    i++;
   }
  }
  node = node->children[i];
  goto insert;
 }
}
# 205 "btree.cilk"
int btree_insert_key(btree *btree, bt_key_val *key_val) {
 bt_node *rnode;

 rnode = btree->root;
 if (rnode->nr_active== (2*btree->order- 1)) {
  bt_node *new_root;
  new_root = allocate_btree_node(btree->order);
  new_root->level = btree->root->level + 1;
  btree->root = new_root;
  new_root->leaf = false;
  new_root->nr_active = 0;
  new_root->children[0] = rnode;
  btree_split_child(btree,new_root,0,rnode);
  btree_insert_nonfull(btree,new_root,key_val);
 } else
  btree_insert_nonfull(btree,rnode,key_val);

        return 0;
}
# 231 "btree.cilk"
static node_pos get_max_key_pos(btree *btree, bt_node *subtree) {
 node_pos node_pos;
 bt_node *node= subtree;

 while (true) {
  if (node== ((void *)0)) {
   break;
  }

  if (node->leaf) {
   node_pos.node = node;
   node_pos.index = node->nr_active - 1;
   return node_pos;
  } else {
   node_pos.node = node;
   node_pos.index = node->nr_active - 1;
   node = node->children[node->nr_active];
  }
 }
 return node_pos;
}
# 259 "btree.cilk"
static node_pos get_min_key_pos(btree *btree, bt_node *subtree) {
 node_pos node_pos;
 bt_node *node= subtree;

 while (true) {
  if (node== ((void *)0)) {
   break;
  }

  if (node->leaf) {
   node_pos.node = node;
   node_pos.index = 0;
   return node_pos;
  } else {
   node_pos.node = node;
   node_pos.index = 0;
   node = node->children[0];
  }
 }
 return node_pos;
}
# 289 "btree.cilk"
static bt_node *merge_siblings (btree *btree, bt_node *parent, unsigned index,
     position_t pos) {
 unsigned i;unsigned j;
 bt_node *new_node;
 bt_node *n1;bt_node*n2;

        if (index == (parent->nr_active)) {
               index--;
        n1 = parent->children[parent->nr_active - 1];
        n2 = parent->children[parent->nr_active];
        } else {
               n1 = parent->children[index];
        n2 = parent->children[index + 1];
        }


 new_node = allocate_btree_node(btree->order);
 new_node->level = n1->level;
 new_node->leaf = n1->leaf;

 for (j=0; j<btree->order-1; j++) {
  new_node->key_vals[j] = n1->key_vals[j];
  new_node->children[j] = n1->children[j];
 }

 new_node->key_vals[btree->order - 1] = parent->key_vals[index];
 new_node->children[btree->order - 1] = n1->children[btree->order - 1];

 for (j=0; j<btree->order-1; j++) {
  new_node->key_vals[j + btree->order] = n2->key_vals[j];
  new_node->children[j + btree->order] = n2->children[j];
 }
 new_node->children[2*btree->order - 1] = n2->children[btree->order - 1];

 parent->children[index] = new_node;

 for (j= index; j<parent->nr_active; j++) {
  parent->key_vals[j] = parent->key_vals[j + 1];
  parent->children[j + 1] = parent->children[j + 2];
 }

 new_node->nr_active = n1->nr_active + n2->nr_active + 1;
 parent->nr_active--;

 for (i=parent->nr_active; i<2*btree->order - 1; i++) {
  parent->key_vals[i] = ((void *)0);
 }

 free_btree_node(n1);
 free_btree_node(n2);

 if (parent->nr_active == 0 && btree->root == parent) {
  free_btree_node(parent);
  btree->root = new_node;
  if (new_node->level)
   new_node->leaf = false;
  else
   new_node->leaf = true;
 }

 return new_node;
}
# 360 "btree.cilk"
static void move_key(btree *btree, bt_node *node, unsigned index, position_t pos) {
 bt_node *lchild;
 bt_node *rchild;
 unsigned i;

 if (pos== right) {
  index--;
 }
 lchild = node->children[index];
 rchild = node->children[index + 1];


 if (pos== left) {
  lchild->key_vals[lchild->nr_active] = node->key_vals[index];
  lchild->children[lchild->nr_active + 1] = rchild->children[0];
  rchild->children[0] = ((void *)0);
  lchild->nr_active++;

  node->key_vals[index] = rchild->key_vals[0];
  rchild->key_vals[0] = ((void *)0);

  for (i=0; i<rchild->nr_active-1; i++) {
   rchild->key_vals[i] = rchild->key_vals[i + 1];
   rchild->children[i] = rchild->children[i + 1];
  }
  rchild->children[rchild->nr_active - 1] =
    rchild->children[rchild->nr_active];
  rchild->nr_active--;
 } else {

  for (i=rchild->nr_active; i>0; i--) {
   rchild->key_vals[i] = rchild->key_vals[i - 1];
   rchild->children[i + 1] = rchild->children[i];
  }
  rchild->children[1] = rchild->children[0];
  rchild->children[0] = ((void *)0);

  rchild->key_vals[0] = node->key_vals[index];

  rchild->children[0] = lchild->children[lchild->nr_active];
  lchild->children[lchild->nr_active] = ((void *)0);

  node->key_vals[index] = lchild->key_vals[lchild->nr_active - 1];
  lchild->key_vals[lchild->nr_active - 1] = ((void *)0);

  lchild->nr_active--;
  rchild->nr_active++;
 }
}
# 416 "btree.cilk"
static bt_node *merge_nodes (btree *btree, bt_node *n1, bt_key_val *kv,
                                                bt_node *n2) {
 bt_node *new_node;
 unsigned i;

 new_node = allocate_btree_node(btree->order);
 new_node->leaf = true;

 for (i=0; i<n1->nr_active; i++) {
  new_node->key_vals[i] = n1->key_vals[i];
                new_node->children[i] = n1->children[i];
 }
        new_node->children[n1->nr_active] = n1->children[n1->nr_active];
        new_node->key_vals[n1->nr_active] = kv;

 for (i=0; i<n2->nr_active; i++) {
  new_node->key_vals[i + n1->nr_active + 1] = n2->key_vals[i];
                new_node->children[i + n1->nr_active + 1] = n2->children[i];
 }
        new_node->children[2*btree->order - 1] = n2->children[n2->nr_active];

        new_node->nr_active = n1->nr_active + n2->nr_active + 1;
        new_node->leaf = n1->leaf;
        new_node->level = n1->level;

 free_btree_node(n1);
 free_btree_node(n2);

 return new_node;
}
# 455 "btree.cilk"
int delete_key_from_node(btree *btree, node_pos *node_pos) {
 unsigned keys_max= 2*btree->order - 1;
 unsigned i;
 bt_key_val *key_val;
 bt_node *node= node_pos->node;

 if (node->leaf== false) {
  return -1;
 }

 key_val = node->key_vals[node_pos->index];

 for (i=node_pos->index; i<keys_max- 1; i++) {
  node->key_vals[i] = node->key_vals[i + 1];
 }

 if (key_val->key) {
  free(key_val->key);
                key_val->key = ((void *)0);
 }

 if (key_val->val) {
  free(key_val->val);
                key_val->val = ((void *)0);
 }

 node->nr_active--;

 if (node->nr_active== 0) {
  free_btree_node(node);
 }
 return 0;
}
# 498 "btree.cilk"
int btree_delete_key(btree *btree, bt_node *subtree, void *key) {
 unsigned i;unsigned index;
 bt_node *node= ((void *)0);bt_node*rsibling;bt_node*lsibling;
 bt_node *comb_node;bt_node*parent;
 node_pos sub_node_pos;
 node_pos node_pos;
 bt_key_val *key_val;bt_key_val*new_key_val;
 unsigned kv= btree->value(key);

 node = subtree;
 parent = ((void *)0);

del_loop:for (i = 0; ; i=0) {


            if (!node->nr_active)
                return -1;
# 519 "btree.cilk"
     while (i < node->nr_active && kv >
       btree->value(node->key_vals[i]->key)) {
      i++;
     }
     index = i;


     if (i< node->nr_active &&
   kv == btree->value(node->key_vals[i]->key)) {
   break;
     }
            if (node->leaf)
                return -1;


     parent = node;


     node = node->children[i];


            if (node == ((void *)0))
                return -1;

            if (index == (parent->nr_active)) {
                lsibling = parent->children[parent->nr_active - 1];
      rsibling = ((void *)0);
            } else if (index == 0) {
                lsibling = ((void *)0);
                rsibling = parent->children[1];
            } else {
         lsibling = parent->children[i - 1];
      rsibling = parent->children[i + 1];
            }

     if (node->nr_active == btree->order - 1 && parent) {


  if (rsibling && (rsibling->nr_active> btree->order - 1)) {
   move_key(btree,parent,i,left);
  } else


  if (lsibling && (lsibling->nr_active> btree->order - 1)) {
   move_key(btree,parent,i,right);
  } else

         if (lsibling&& (lsibling->nr_active== btree->order - 1)) {
          node = merge_siblings(btree,parent,i,left);
  } else

         if (rsibling&& (rsibling->nr_active== btree->order - 1)) {
          node = merge_siblings(btree,parent,i,right);
  }
     }
        }
# 579 "btree.cilk"
 if (node->leaf&& (node->nr_active> btree->order - 1)) {
  node_pos.node = node;
  node_pos.index = index;
  delete_key_from_node(btree,&node_pos);
  return 0;
 }
# 588 "btree.cilk"
 if (node->leaf&& (node== btree->root)) {
  node_pos.node = node;
  node_pos.index = index;
  delete_key_from_node(btree,&node_pos);
  return 0;
 }
# 597 "btree.cilk"
 if (node->leaf== false) {
  if (node->children[index]->nr_active> btree->order - 1) {
   sub_node_pos = get_max_key_pos(btree,node->children[index]);
                        key_val = sub_node_pos.node->key_vals[sub_node_pos.index];

                        new_key_val = (bt_key_val *)malloc(sizeof(bt_key_val));
                        copy_key_val(btree,key_val,new_key_val);
          node->key_vals[index] = new_key_val;

                        btree_delete_key(btree,node->children[index],key_val->key);
   if (sub_node_pos.node->leaf== false) {
                                printf("Not leaf\n");
                        }
  } else if ((node->children[index + 1]->nr_active> btree->order - 1)) {
   sub_node_pos =
                                get_min_key_pos(btree,node->children[index + 1]);
                        key_val = sub_node_pos.node->key_vals[sub_node_pos.index];

                        new_key_val = (bt_key_val *)malloc(sizeof(bt_key_val));
                        copy_key_val(btree,key_val,new_key_val);
          node->key_vals[index] = new_key_val;

                        btree_delete_key(btree,node->children[index + 1],key_val->key);
   if (sub_node_pos.node->leaf== false) {
                                printf("Not leaf\n");
                        }

  } else if (
   node->children[index]->nr_active == btree->order - 1 &&
   node->children[index + 1]->nr_active == btree->order - 1) {

   comb_node = merge_nodes(btree,node->children[index],
                                node->key_vals[index],
    node->children[index + 1]);
   node->children[index] = comb_node;

   for (i=index+ 1; i<node->nr_active; i++) {
    node->children[i] = node->children[i + 1];
    node->key_vals[i - 1] = node->key_vals[i];
   }
   node->nr_active--;
                        if (node->nr_active == 0 && btree->root == node) {
                                free_btree_node(node);
                                btree->root = comb_node;
                        }
                        node = comb_node;
                        goto del_loop;
  }
          }
# 652 "btree.cilk"
 if (node->leaf&& (node->nr_active> btree->order - 1)) {
       node_pos.node = node;
       node_pos.index = index;
       delete_key_from_node(btree,&node_pos);
 }

        return 0;
}
# 667 "btree.cilk"
node_pos get_btree_node(btree *btree, void *key) {
 node_pos kp;
 unsigned key_val= btree->value(key);
 bt_node *node;
 unsigned i= 0;

 node = btree->root;


 for (; ; i=0) {
# 681 "btree.cilk"
     while (i < node->nr_active && key_val >
       btree->value(node->key_vals[i]->key)) {
      i++;
     }


     if (i< node->nr_active &&
   key_val == btree->value(node->key_vals[i]->key)) {
      kp.node = node;
      kp.index = i;
      return kp;
     }
# 696 "btree.cilk"
     if (node->leaf) {
      return kp;
     }


     node = node->children[i];
 }


}
# 712 "btree.cilk"
void btree_destroy(btree *btree) {
       int i= 0;
       unsigned current_level;

       bt_node *head;bt_node*tail;bt_node*node;
       bt_node *child;bt_node*del_node;

       node = btree->root;
       current_level = node->level;
       head = node;
       tail = node;

       while (true) {
               if (head== ((void *)0)) {
                       break;
               }
               if (head->level < current_level) {
                       current_level = head->level;
               }

               if (head->leaf== false) {
                       for (i= 0; i < head->nr_active + 1; i++) {
                               child = head->children[i];
                               tail->next = child;
                               tail = child;
                               child->next = ((void *)0);
                       }
               }
               del_node = head;
               head = head->next;
               free_btree_node(del_node);
       }

}
# 753 "btree.cilk"
bt_key_val *btree_search (btree *btree, void *key) {

 bt_key_val *key_val= ((void *)0);
 node_pos kp= get_btree_node(btree,key);

 if (kp.node) {
  key_val = kp.node->key_vals[kp.index];
 }
 return key_val;
}
# 770 "btree.cilk"
static void copy_key_val(btree *btree, bt_key_val *src, bt_key_val *dst) {
        unsigned keysize;
        unsigned datasize;

        keysize = btree->key_size(src->key);
        dst->key = (void *)malloc(keysize);
        bcopy(src->key,dst->key,keysize);

        if (src->val) {
                datasize = btree->data_size(src->val);
                dst->val = (void *)malloc(datasize);
                bcopy(src->val,dst->val,datasize);
        }

}
# 791 "btree.cilk"
void *btree_get_max_key (btree *btree) {
 node_pos node_pos;
 node_pos = get_max_key_pos(btree,btree->root);
 return node_pos.node->key_vals[node_pos.index]->key;
}
# 802 "btree.cilk"
void *btree_get_min_key (btree *btree) {
 node_pos node_pos;
 node_pos = get_min_key_pos(btree,btree->root);
 return node_pos.node->key_vals[node_pos.index]->key;
}
# 874 "btree.cilk"
unsigned value (void *key) {
    return *((int *)key);
}
# 878 "btree.cilk"
unsigned keysize (void *key) {
        return sizeof(int);
}
# 882 "btree.cilk"
unsigned datasize (void *data) {
        return sizeof(int);
}
# 886 "btree.cilk"
void usage(void){}

static struct option long_options[]={{"nonrecursive",0,&recursive_flag,0},{"parallel",0,&parallel_flag,1},{"count",1,0,'c'},{"order",1,0,'o'},{"num_lookups",1,0,'n'},{0,0,0,0}};struct _cilk_cilk_main_frame{CilkStackFrame header;struct{int argc;char**argv;}scope0;struct{int i;int c;btree*tree;bt_key_val*kv;int item;int count;int order;int*values;int num_lookups;}scope1;struct{int option_index;}scope2;};struct _cilk_cilk_main_args{int _cilk_proc_result;int argc;char**argv;};static void _cilk_cilk_main_slow(CilkWorkerState*const _cilk_ws,struct _cilk_cilk_main_frame*_cilk_frame);static CilkProcInfo _cilk_cilk_main_sig[]={{sizeof(int),sizeof(struct _cilk_cilk_main_frame),_cilk_cilk_main_slow,0,0}};
# 888 "btree.cilk"
int cilk_main(CilkWorkerState*const _cilk_ws,int argc,char**argv){struct _cilk_cilk_main_frame*_cilk_frame;{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_cilk_main_frame), _cilk_cilk_main_sig); };{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_event_new_thread_maybe(_cilk_ws); };
{
    int i= 0;
    int c;
    btree *tree;
    bt_key_val *kv;
    int item= 0x43;
    int count;
    int order;
    int *values;
    int num_lookups;

    srandom(0);

    count = 134217728;
    order = 5;
    num_lookups = 100;

    while (1) {
# 922 "btree.cilk"
        int _cilk_temp0= 0;void*_cilk_temp1=((_cilk_frame->scope2.option_index=_cilk_temp0),&_cilk_temp1);

        c = getopt_long (argc, argv, "c:o:n:",
                         long_options, &_cilk_frame->scope2.option_index);


        if (c == -1)
            break;

        switch (c) {
            case 0:

                if (long_options[_cilk_frame->scope2.option_index].flag!=0)
                    break;
                printf ("option %s", long_options[_cilk_frame->scope2.option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case 'c':
                printf ("option -c with value `%s'\n", optarg);
                count = atoi(optarg);
                break;

            case 'o':
                printf ("option -o with value `%s'\n", optarg);
                order = atoi(optarg);
                break;

            case 'n':
                printf ("option -n with value `%s'\n", optarg);
                num_lookups = atoi(optarg);

            case '?':

                break;

            default:
                usage ();
        }
    }


    if (optind < argc) {
        printf ("non-option ARGV-elements: ");
        while (optind < argc) {
            printf ("%s ", argv[optind++]);
        }
        putchar ('\n');
    }
    values = (int *)malloc(num_lookups*sizeof(int));

    tree = btree_create(order);
        tree->value = value;
        tree->key_size = keysize;
        tree->data_size = datasize;

    for (i=0; i<count; i++) {
        kv = (bt_key_val*)malloc(sizeof((*kv)));
        kv->key = malloc(sizeof(int));
        *(int *)kv->key = i;
        kv->val = malloc(sizeof(int));
        *(int *)kv->val = i;
        btree_insert_key(tree,kv);
    }

    for (i= 0; i<num_lookups; i++) {
        values[i] = random()%count;
    }

    gettimeofday(&begin, ((void *)0));

    for (i= 0; i<num_lookups; ++i) {
        btree_search(tree, &values[i]);
        printf("found %d\n", values[i]);
    }

    gettimeofday(&end, ((void *)0));
    time_spent = end.tv_sec*1000000+end.tv_usec
        -(begin.tv_sec*1000000+begin.tv_usec);

    printf("avg_lookup(us): %f\n", time_spent/num_lookups);

    free(values);
    btree_destroy(tree);

    {int _cilk_temp2=0;{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return _cilk_temp2;}
}}
# 888 "btree.cilk"
static void _cilk_cilk_main_slow(CilkWorkerState*const _cilk_ws,struct _cilk_cilk_main_frame*_cilk_frame){int argc;char**argv;{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header)); };switch (_cilk_frame->header.entry) {}argc=_cilk_frame->scope0.argc;argv=_cilk_frame->scope0.argv;
{
    int i= 0;
    int c;
    btree *tree;
    bt_key_val *kv;
    int item= 0x43;
    int count;
    int order;
    int *values;
    int num_lookups;

    srandom(0);

    count = 134217728;
    order = 5;
    num_lookups = 100;

    while (1) {
# 922 "btree.cilk"
        int _cilk_temp3= 0;void*_cilk_temp4=((_cilk_frame->scope2.option_index=_cilk_temp3),&_cilk_temp4);

        c = getopt_long (argc, argv, "c:o:n:",
                         long_options, &_cilk_frame->scope2.option_index);


        if (c == -1)
            break;

        switch (c) {
            case 0:

                if (long_options[_cilk_frame->scope2.option_index].flag!=0)
                    break;
                printf ("option %s", long_options[_cilk_frame->scope2.option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case 'c':
                printf ("option -c with value `%s'\n", optarg);
                count = atoi(optarg);
                break;

            case 'o':
                printf ("option -o with value `%s'\n", optarg);
                order = atoi(optarg);
                break;

            case 'n':
                printf ("option -n with value `%s'\n", optarg);
                num_lookups = atoi(optarg);

            case '?':

                break;

            default:
                usage ();
        }
    }


    if (optind < argc) {
        printf ("non-option ARGV-elements: ");
        while (optind < argc) {
            printf ("%s ", argv[optind++]);
        }
        putchar ('\n');
    }
    values = (int *)malloc(num_lookups*sizeof(int));

    tree = btree_create(order);
        tree->value = value;
        tree->key_size = keysize;
        tree->data_size = datasize;

    for (i=0; i<count; i++) {
        kv = (bt_key_val*)malloc(sizeof((*kv)));
        kv->key = malloc(sizeof(int));
        *(int *)kv->key = i;
        kv->val = malloc(sizeof(int));
        *(int *)kv->val = i;
        btree_insert_key(tree,kv);
    }

    for (i= 0; i<num_lookups; i++) {
        values[i] = random()%count;
    }

    gettimeofday(&begin, ((void *)0));

    for (i= 0; i<num_lookups; ++i) {
        btree_search(tree, &values[i]);
        printf("found %d\n", values[i]);
    }

    gettimeofday(&end, ((void *)0));
    time_spent = end.tv_sec*1000000+end.tv_usec
        -(begin.tv_sec*1000000+begin.tv_usec);

    printf("avg_lookup(us): %f\n", time_spent/num_lookups);

    free(values);
    btree_destroy(tree);

    {{int __tmp=0;Cilk_set_result(_cilk_ws,&__tmp,sizeof((__tmp)));}{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame)); };return;}
}}
# 888 "btree.cilk"
static void _cilk_cilk_main_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v)
{(void)_cilk_ws;(void)_cilk_procargs_v;((struct _cilk_cilk_main_args*)_cilk_procargs_v)->_cilk_proc_result=cilk_main(_cilk_ws,((struct _cilk_cilk_main_args*)_cilk_procargs_v)->argc,((struct _cilk_cilk_main_args*)_cilk_procargs_v)->argv);
# 1010 "btree.cilk"
}
# 888 "btree.cilk"
int mt_cilk_main(CilkContext*const context,int argc,char**argv)
{struct _cilk_cilk_main_args*_cilk_procargs;
# 888 "btree.cilk"
int _cilk_proc_result;_cilk_procargs=(struct _cilk_cilk_main_args*)Cilk_malloc_fixed(sizeof(struct _cilk_cilk_main_args));_cilk_procargs->argc=argc;_cilk_procargs->argv=argv;Cilk_start(context,_cilk_cilk_main_import,_cilk_procargs,sizeof(int));_cilk_proc_result=_cilk_procargs->_cilk_proc_result;Cilk_free(_cilk_procargs);return _cilk_proc_result;
# 1010 "btree.cilk"
}