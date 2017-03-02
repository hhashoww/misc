#ifndef ARCH_H
#define ARCH_H

#include <stdio.h>

#include "types.h"

// 32/64 bits
//#define BITS_PER_LONG			32
#define BITS_PER_LONG			64

// cache line size in bytes
#define CACHE_LINE				64

// big/small endian
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN
#endif
//#ifndef BIG_ENDIAN
//#define BIG_ENDIAN
//#endif

// environment
// #define VC_X86
#define GCC_X86
// #define GCC_MIPS

#ifdef VC_X86

#include <intrin.h>
#include <Windows.h>
#include <Winsock2.h>
// compiler magic
	#pragma intrinsic(_ReadWriteBarrier)
	// Use __thread to declare a thread variable.
	#define __thread					__declspec( thread )
	// Use __func__ to get the unadorned name of the function.
	#define __func__					__FUNCTION__
	#define inline						_inline
	// barrier() prevent compiler re-ordering memory across barrier
	#define barrier()					_ReadWriteBarrier()
	// use likely() and unlikely() to provide the compiler with branch prediction information
	#define likely(x)					(!!(x))
	#define unlikely(x)					(!!(x))
	// prefetch() indicates to the processor that a cache line will be needed in the near future.
	#define prefetch(p)					(p)
#include <malloc.h>
	// aligned_malloc() allocates memory on a specified alignment boundary
	#define aligned_malloc(size, alignment)		_aligned_malloc(size, alignment)
	// aligned_malloc() frees a block of memory that was allocated with _aligned_malloc
	#define aligned_free(ptr)					_aligned_free(ptr)

// synchronization operations
	#pragma intrinsic(_mm_mfence,_mm_lfence,_mm_sfence,_InterlockedExchange,_bittestandset)
	// mb() barrier ensures that memory read and write operations that appear before the memory
	// barrier instruction are committed to memory (not necessary immediately) before any memory
	// read and write operations that appear after the memory barrier instruction. A read memory
	// barrier orders only the memory read operations and a write memory barrier orders only the
	// memory write operations.
	#define mb()						_mm_mfence()
	#define rmb()						_mm_lfence()
	#define wmb()						_mm_sfence()
	#define smp_mb()					mb()
	#define smp_rmb()					rmb()
	#define smp_wmb()					wmb()
	// mc(), rmc(), wmc() are stronger than mb(), rmb(), wmb() because they ensures prior memory
	// operations are complete and visible to other processors before subsequence memory
	// operations. (usually done by flush cache)
	// Intel processor use coherent memory with write-through architec, thus there is no need to
	// flush cache.
	#define mc()						mb()
	#define rmc()						rmb()
	#define wmc()						wmb()
	#define smp_mc()					mc()
	#define smp_rmc()					rmc()
	#define smp_wmc()					wmc()

// rwlock_t operations
	typedef SRWLOCK						rwlock_t;
	#define DEFINE_RWLOCK(x)			rwlock_t x = SRWLOCK_INIT
	#define rwlock_init(lock)			InitializeSRWLock(lock)
	#define read_trylock(lock)			TryAcquireSRWLockShared(lock)		// need windows 7
	#define read_lock(lock)				AcquireSRWLockShared(lock)
	#define read_unlock(lock)			ReleaseSRWLockShared(lock)
	#define write_trylock(lock)			TryAcquireSRWLockExclusive(lock)	// need windows 7
	#define write_lock(lock)			AcquireSRWLockExclusive(lock)
	#define write_unlock(lock)			ReleaseSRWLockExclusive(lock)

// spin_lock_t operations
	typedef SRWLOCK						spinlock_t;
	// macro to delcare and initialize spinlock
	#define DEFINE_SPINLOCK(x)			spinlock_t x = SRWLOCK_INIT
	#define spin_lock_init(lock)		InitializeSRWLock(lock)
	#define spin_trylock(lock)			TryAcquireSRWLockExclusive(lock)	// need windows 7
	#define spin_lock(lock)				AcquireSRWLockExclusive(lock)
	#define spin_unlock(lock)			ReleaseSRWLockExclusive(lock)

// atomic operations
	#define xchg(addr, v)				_InterlockedExchange(addr, v)
	typedef struct {
		volatile long		counter;
	} atomic_t;
	static inline int atomic_read(atomic_t *v)			{ return (v->counter); }
	static inline void atomic_set(atomic_t *v, int i)	{ v->counter = i; }
	static inline void atomic_inc(atomic_t *v)			{ InterlockedIncrement(&v->counter); }
	static inline void atomic_dec(atomic_t *v)			{ InterlockedDecrement(&v->counter); }

	static inline void *atomic_xchg_ptr(void **v, void *new)	{ return InterlockedExchangePointer(v, new); }
	static inline int atomic_bit_test_and_set(long *v, long pos)	{ return _bittestandset(v, pos); }

// simulation of pthread library
typedef long		pthread_t;
typedef long		pthread_attr_t;

static inline int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg)
{
	return (!(thread=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, NULL)));
}

typedef SRWLOCK		pthread_mutex_t;
typedef void		pthread_mutexattr_t;

#define PTHREAD_MUTEX_INITIALIZER		SRWLOCK_INIT

static int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
	InitializeSRWLock(mutex);
	return 0;
}

static inline int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
	return 0;
}

static inline int pthread_mutex_lock(pthread_mutex_t *mutex)
{
	AcquireSRWLockExclusive(mutex);
	return 0;
}

/* // need windows 7
static inline int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
	return !TryAcquireSRWLockExclusive(mutex);
}
*/

static inline int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	ReleaseSRWLockExclusive(mutex);
	return 0;
}

static inline void pthread_yield(void)
{
	Sleep(1);
}

#define sleep(seconds)				Sleep(seconds*1000)

#endif // VC_X86

#ifdef GCC_X86
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
	// barrier() prevent compiler re-ordering memory across barrier
	#define barrier()					 asm volatile("" : : : "memory");
	// Use offsetof() to get the offset of struct member
//	#define offsetof(type, member)		__builtin_offsetof (type, member)
	// use likely() and unlikely() to provide the compiler with branch prediction information
	#define likely(x)					__builtin_expect(!!(x), 1)
	#define unlikely(x)					__builtin_expect(!!(x), 0)
	// prefetch() indicates to the processor that a cache line will be needed in the near future.
	#define prefetch(p)					__builtin_prefetch(p)
	// aligned_member
	#define __align_cache				__attribute__ ((aligned (CACHE_LINE)))

	// aligned_malloc() allocates memory on a specified alignment boundary
static inline void *aligned_malloc(size_t size, size_t alignment) {
  void *memptr;
  posix_memalign(&memptr, alignment, size);
  return memptr;
}
  // aligned_malloc() frees a block of memory that was allocated with _aligned_malloc
	#define aligned_free(ptr)					free(ptr)

  // mb() barrier ensures that memory read and write operations that appear before the memory
	// barrier instruction are committed to memory (not necessary immediately) before any memory
	// read and write operations that appear after the memory barrier instruction. A read memory
	// barrier orders only the memory read operations and a write memory barrier orders only the
	// memory write operations.
	#define mb()						asm volatile("mfence":::"memory")
	#define rmb()						asm volatile("lfence":::"memory")
	#define wmb()						asm volatile("sfence"::: "memory")
	#define smp_mb()					mb()
	#define smp_rmb()					rmb()
	#define smp_wmb()					wmb()

	#define mc()						mb()
	#define rmc()						rmb()
	#define wmc()						wmb()
	#define smp_mc()					mc()
	#define smp_rmc()					rmc()
	#define smp_wmc()					wmc()

// rwlock_t operations
	typedef pthread_rwlock_t			rwlock_t;
	#define DEFINE_RWLOCK(x)			rwlock_t x = PTHREAD_RWLOCK_INITIALIZER
	#define rwlock_init(lock)			pthread_rwlock_init(lock, NULL)
	#define read_trylock(lock)			pthread_rwlock_tryrdlock(lock)
	#define read_lock(lock)				pthread_rwlock_rdlock(lock)
	#define read_unlock(lock)			pthread_rwlock_unlock(lock)
	#define write_trylock(lock)			pthread_rwlock_trywrlock(lock)
	#define write_lock(lock)			pthread_rwlock_wrlock(lock)
	#define write_unlock(lock)			pthread_rwlock_unlock(lock)

// spin_lock_t operations
	typedef pthread_spinlock_t			spinlock_t;
	// macro to delcare and initialize spinlock
//	#define DEFINE_SPINLOCK(x)			spinlock_t x = PTHREAD_MUTEX_INITIALIZER
	#define spin_lock_init(lock)		pthread_spin_init(lock, PTHREAD_PROCESS_PRIVATE)
	#define spin_trylock(lock)			pthread_spin_trylock(lock)
	#define spin_lock(lock)				pthread_spin_lock(lock)
	#define spin_unlock(lock)			pthread_spin_unlock(lock)

// atomic operations
static inline void *atomic_xchg_ptr(void **v, void *new) {
  return __sync_lock_test_and_set(v, new);
}

static __inline__ int atomic_bit_test_and_set(unsigned long * a, long b)
{
	unsigned char retval;

	if(__builtin_constant_p(b))
		__asm__("bts %[b], %[a]; setb %b[retval]" : [retval] "=q" (retval) : [a] "mr" (*(a + (b / BITS_PER_LONG))), [b] "Ir" (b % BITS_PER_LONG));
	else
		__asm__("bts %[b], %[a]; setb %b[retval]" : [retval] "=q" (retval) : [a] "mr" (*a), [b] "r" (b));

	return retval;
}

static __inline__ int atomic_bit_test_and_reset(unsigned long * a, long b)
{
	unsigned char retval;

	if(__builtin_constant_p(b))
		__asm__("btr %[b], %[a]; setb %b[retval]" : [retval] "=q" (retval) : [a] "mr" (*(a + (b / BITS_PER_LONG))), [b] "Ir" (b % BITS_PER_LONG));
	else
		__asm__("btr %[b], %[a]; setb %b[retval]" : [retval] "=q" (retval) : [a] "mr" (*a), [b] "r" (b));

	return retval;
}

/*
static inline void pthread_yield(void)
{
	usleep(1000);
}
*/
#endif

// Instruct the compiler to perform only a single access to a variable (prohibits merging and refetching).
#define ACCESS_ONCE(x)		(*(volatile typeof(x) *)&x)
// Get the container address according to member address &ptr
#define container_of(ptr, type, member)	((type *)( (char *)(ptr) - offsetof(type, member) ))

// network <-> host translation
#define ntoh32(v)					ntohl(v)
#define hton32(v)					htonl(v)
#define ntoh16(v)					ntohs(v)
#define hton16(v)					htons(v)

#ifdef LITTLE_ENDIAN
#define ntoh64(v)					__builtin_bswap64(v)
#define	hton64(v)					__builtin_bswap64(v)
#else	// BIG_ENDIAN
#define ntoh64(v)					(v)
#define hton64(v)					(v)
#endif

// error handling
#define SUCCESS		1
#define FAIL		0
#define TRUE		1
#define FALSE		0

#define MYASSERT(cond)				if (!(cond)) {	printf("ERROR:%s(): %d\n", __func__, __LINE__); return FAIL; }

// for dump
static inline void printSpace(int num)
{
	while (num>0) {
		printf(" ");
		num--;
	}
}

// thread

#ifdef VC_X86
#include <windows.h>

static _inline void thread_create(void *(*start_routine)(void*), void *arg)
{
	LPTHREAD_START_ROUTINE	f = (LPTHREAD_START_ROUTINE)start_routine;
	CreateThread(NULL, 0, f, arg, 0, NULL);
}

#endif	// VC_X86

#endif	// ARCH_H
