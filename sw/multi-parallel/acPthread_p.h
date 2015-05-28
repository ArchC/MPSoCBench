#ifndef acPthread_H_
#define acPthread_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../defines.h"

#define MAX_ESL_PTHREADS 64

#define PTHREAD_CREATE_JOINABLE 0    // for compatibility

#define AcquireGlobalLock()	while (*lock)
#define ReleaseGlobalLock()	((*lock)=0)

#define DEBUG 0

/***********************************************************************************
                       STRUCTURES 
***********************************************************************************/

typedef struct pthread_attr_tag {
//int attr_sw;
}pthread_attr_t;

typedef struct pthread_tag {
//int index;
}pthread_t;

typedef struct pthread_mutexattr_tag {
}pthread_mutexattr_t;

typedef struct pthread_mutex_tag {
        int value;
} pthread_mutex_t;

typedef struct barrier_tag {
	int valid;
	int initial_counter;
        int counter;
    	int ctrl;
} pthread_barrier_t;

typedef struct pthread_join_tag {
	int num_proc;
	int counter_join_app; 
	int thread_main0_finished;
	int thread_main0_dijkstra_finished;
	int thread_main0_sha_finished;
	int thread_main0_basicmath_finished;
	int thread_main0_stringsearch_finished;
} pthread_join_t;

typedef struct pthread_cond_tag {
int wake;
}pthread_cond_t;

typedef struct pthread_condattr_tag{
}pthread_condattr_t;

typedef struct ThreadNode_t
{
	void (*functionPointer);          // function pointer
	void *arg;
	int attr;


}ThreadNode;

typedef struct ThreadQueue_t
{
	int begin; 
	int end;
	ThreadNode threads[MAX_ESL_PTHREADS];
	int numberOfElements;
} ThreadQueue;

/***********************************************************************************
                                   FUNCTIONS
***********************************************************************************/

/*  QUEUE */
ThreadNode initNode( ThreadNode*, void (*fp)(void *), void*); //, int);
void initQueue (ThreadQueue*);
void printQueue (ThreadQueue*);
int isEmpty(ThreadQueue*);
void enqueue (ThreadQueue*,ThreadNode*);
ThreadNode *dequeue (ThreadQueue*);
void destroy(ThreadQueue*);
int size(ThreadQueue*);

/*  acPthread JUST FOR COMPATIBILITY  */
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
int pthread_attr_init(pthread_attr_t *attr);
void pthread_exit(void *);   // verificar qual é o argumetno correto na biblioteca original


/*  acPthread */
void pthread_my_exit(); 
void pthread_busywait(int);
int pthread_join_init (pthread_join_t *);
void pthread_join_control (pthread_join_t *);
void pthread_join(pthread_t, const pthread_attr_t *); 
void pthread_mutex_init(pthread_mutex_t *, const pthread_mutexattr_t *);
int pthread_mutex_lock(pthread_mutex_t *);
int pthread_mutex_unlock(pthread_mutex_t *);
void pthread_mutex_destroy (pthread_mutex_t *m_lock);
int pthread_init();
void pthread_executeThread();
int pthread_create(pthread_t *, const pthread_attr_t *, void (*start_routine)(void *), void *);
int pthread_cond_init (pthread_cond_t *, const pthread_condattr_t *);
int pthread_cond_wait (pthread_cond_t *, pthread_mutex_t *);
int pthread_cond_broadcast(pthread_cond_t *);
void pthread_barrier_init (pthread_barrier_t *, int);
int pthread_barrier_wait (pthread_barrier_t *);

/* join auxiliaries functions */

/*void join_sha (pthread_t thread, const pthread_attr_t *attr);
void join_stringsearch(pthread_t thread, const pthread_attr_t *attr);
void join_basicmath(pthread_t thread, const pthread_attr_t *attr);
void join_dijkstra(pthread_t thread, const pthread_attr_t *attr);
*/

void *acPthread_malloc(size_t);
void *acPthread_realloc(void *, size_t);
void *acPthread_calloc(size_t , size_t );
void acPthread_free(void *);

/* DVFS function */

void pthread_changePowerState(int);
void pthread_turnOnProcessors();
void pthread_turnOffProcessors();

#endif
