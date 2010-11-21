#ifndef __PTHREAD_INTERNAL_H__
#define __PTHREAD_INTERNAL_H__

#include <rtthread.h>
#include <pthread.h>

struct _pthread_cleanup
{
	void (*cleanup_func)(void* parameter);
	void* parameter;

	struct _pthread_cleanup* next;
};
typedef struct _pthread_cleanup _pthread_cleanup_t;

struct _pthread_key_data
{
	int is_used;
	void (*destructor)(void* parameter);
};
typedef struct _pthread_key_data _pthread_key_data_t;

#define PTHREAD_MAGIC	0x70746873
struct _pthread_data
{
	rt_uint32_t magic;
	pthread_attr_t attr;
	rt_thread_t tid;

	void* (*thread_entry)(void* parameter);
	void* thread_parameter;

	/* return value */
	void* return_value;

	/* semaphore for joinable thread */
	rt_sem_t joinable_sem;

	/* cancel state and type */
	rt_uint8_t cancelstate;
	volatile rt_uint8_t canceltype;
	volatile rt_uint8_t canceled;

	_pthread_cleanup_t* cleanup;
	void** tls; /* thread-local storage area */
};
typedef struct _pthread_data _pthread_data_t;

rt_inline _pthread_data_t* _pthread_get_data(pthread_t thread)
{
	RT_ASSERT(thread != RT_NULL);

	return (_pthread_data_t*)thread->user_data;
}

#define NSEC_PER_TICK	(1000000000UL/RT_TICK_PER_SECOND)

#endif