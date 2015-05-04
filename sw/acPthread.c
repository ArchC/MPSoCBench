/********************************************************************************
	MPSoCBench Benchmark Suite
	Authors: Liana Duenha
	Supervisor: Rodolfo Azevedo
	Date: July-2012
	www.archc.org/benchs/mpsocbench

	Computer Systems Laboratory (LSC)
	IC-UNICAMP
	http://www.lsc.ic.unicamp.br/


	This source code is part of the MPSoCBench Benchmark Suite, which is a free
	source-code benchmark for evaluation of Electronic Systemc Level designs.
	This benchmark is distributed with hope that it will be useful, but
	without any warranty.

*********************************************************************************/


#ifndef acPthread_H_
#include "acPthread.h"
#endif

/***********************************************************************************
                       STRUCTURES AND GLOBAL VARIABLES
***********************************************************************************/

unsigned volatile int *lock = (unsigned volatile int *)LOCK_ADDRESS;
unsigned volatile int *dfs = (unsigned volatile int *)DVFS_ADDRESS;
unsigned volatile int *intr_ctrl = (unsigned volatile int *)INTR_CTRL_ADDRESS;


ThreadQueue tQueue;  // threads queue
int pthread_n_workers; // number of processors or threads, given as a main argument (argv[1])
int pthread_n_softwares; // number of softwares in the multisoftware platform
int pthread_threads_per_software; // number of threads per software 


int pthread_created = 0;
int pthread_ended;

//int flag2 = 1; 

//pthread_mutex_t mutex_start;
pthread_mutex_t mutex_print;
pthread_mutex_t mutex_malloc;

pthread_join_t join;
int pthread_numberOfActiveThreads = 0; 


int pthread_finished = 0;

int pthread_free_for_start = 0;

int pthread_counter_procs_finished;

int join_out = 0;


/***********************************************************************************
                                   FUNCTIONS
***********************************************************************************/

/***********************************************************************************
QUEUE FUNCTIONS
***********************************************************************************/

void initNode( ThreadNode *node, void (*fp)(void *), void *arg) //, int attr)
{
	node->arg = arg;
	node->functionPointer = fp;
	
	return;
}


void initQueue(ThreadQueue *q)
{
	AcquireGlobalLock();
	q->begin = -1;
	q->end = -1;
	q->numberOfElements = 0;
	ReleaseGlobalLock();
	
	return;
}


void printQueue (ThreadQueue *q)
{
	pthread_mutex_lock(&mutex_print);

    if (DEBUG)
	{
		printf("\nPrintQueue(): \n");
		printf("\nThreads queue with %d threads: \n", q->numberOfElements);

	}

	pthread_mutex_unlock(&mutex_print);
	return;
}


int isEmpty(ThreadQueue *q)
{

	if (q->numberOfElements == 0) return 1;

	return 0;
}


void enqueue (ThreadQueue *q, ThreadNode *node)
{
	if (isEmpty(q))
	{
		(q->begin)++;
		(q->end)++;
		q->threads[q->end] = *node;
	
	}
	else
        {
		(q->end)++;
		q->threads[q->end] = *node;
	}
	q->numberOfElements++;
}

ThreadNode *dequeue (ThreadQueue *q)
{

	if ( !(isEmpty(q)) )
	{
		int localbegin = q->begin;
		if (q->begin == q->end) 
		{ 
			q->begin = q->end = -1; 
		}
		else q->begin ++;
		q->numberOfElements--;
		
		return &(q->threads[localbegin]);
	}
	printf("\nEmpty Queue!\n");

	return NULL;
}


void destroy(ThreadQueue *q)
{
	
}

int size(ThreadQueue *q)
{
	return q->numberOfElements;
}

/***********************************************************************************
END OF THE QUEUE FUNCTIONS
***********************************************************************************/


/***********************************************************************************
ESL_PTHREAD FUNCTIONS IMPLEMENTED JUST FOR COMPATIBILITY
***********************************************************************************/

int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
	return 0; 
}
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate)
{
	
	return 0;
}
int pthread_attr_init(pthread_attr_t *attr)
{
}
void pthread_exit(void *ret_val)
{

}
void pthread_my_exit()  
{
	AcquireGlobalLock();	
	pthread_finished = 1;
	ReleaseGlobalLock();

	if(DEBUG)
	{
		pthread_mutex_lock(&mutex_print);
		printf("\nAt the end of pthread_my_exit...\n");
        pthread_mutex_unlock(&mutex_print);
	}
}

/***********************************************************************************
END OF ESL_PTHREAD FUNCTIONS IMPLEMENTED JUST FOR COMPATIBILITY
************************************************************************************/

/*---------------------------JOIN FUNCTIONS--------------------------------------- */
int pthread_join_init (pthread_join_t *m_join)
{

	AcquireGlobalLock();

	m_join->num_proc = pthread_n_workers;
	pthread_ended = pthread_n_workers;

	ReleaseGlobalLock();
	
	return 0;

}

void pthread_join_control (pthread_join_t *m_join) 
{
	

	AcquireGlobalLock();
	m_join->num_proc--;
	printf ("\nJoin Point->%d of %d processors are not over yet...",m_join->num_proc,pthread_n_workers);
	if (m_join->num_proc ==0)
	{
		join_out = 1;
	}
	
	ReleaseGlobalLock();
	
	
	while (join_out == 0);

	pthread_busywait(500);
	
	if(DEBUG)
	{
		pthread_mutex_lock(&mutex_print);
		printf("\nAt the end of join_control...\n");
        pthread_mutex_unlock(&mutex_print);
	}

}
void pthread_join(pthread_t thread, const pthread_attr_t *attr) 
{

}


/*---------------------------ENDING JOIN FUNCTIONS------------------------------------ */



/*---------------------------MUTEX FUNCTIONS------------------------------------------ */

void pthread_mutex_init(pthread_mutex_t *m_lock, const pthread_mutexattr_t *attr)
{
	AcquireGlobalLock();
	m_lock->value = 0;
	ReleaseGlobalLock();
}

int pthread_mutex_lock(pthread_mutex_t *m_lock)
{
	int status, i;	
	AcquireGlobalLock();

	//Trying to acquire the lock state
	while (m_lock->value == 1){
		ReleaseGlobalLock();
		pthread_busywait(500);
		AcquireGlobalLock();
		
	}

	//Acquiring the lock state
	m_lock->value = 1;

	ReleaseGlobalLock();

	status = 0;

	return status; 
}


int pthread_mutex_unlock(pthread_mutex_t *m_lock)
{
	AcquireGlobalLock();
	m_lock->value = 0;
	ReleaseGlobalLock();
	return 0;
}


void pthread_mutex_destroy (pthread_mutex_t *m_lock)
{
	free(m_lock);
}

/*---------------------------ENDING MUTEX FUNCTIONS------------------------------------- */

/*----------------------------  PTHREAD FUNCTIONS--------------------------------------- */



void pthread_busywait(int value)
{
	int i;
	for (i=0; i<value; i++);
}



int pthread_init ()
{


	initQueue(&tQueue);
	pthread_join_init(&join);

	AcquireGlobalLock();
	pthread_numberOfActiveThreads = 0;
	pthread_free_for_start = 0;
	pthread_finished = 0;
	ReleaseGlobalLock();

	pthread_turnOnProcessors();	
}

void pthread_executeThread ()
{

	AcquireGlobalLock();	
	pthread_numberOfActiveThreads++;

	int local = pthread_numberOfActiveThreads;
	
	if (pthread_numberOfActiveThreads == pthread_n_workers)
	{
		//printf("\nAll %d threads started...\n",pthread_numberOfActiveThreads);

		/**/pthread_numberOfActiveThreads = 0;/**/

		pthread_free_for_start = 1;    
		pthread_created = 0;
		
	}
	ReleaseGlobalLock();

	while(pthread_free_for_start == 0);
		
	AcquireGlobalLock();
	ThreadNode *node = dequeue(&tQueue);	
	ReleaseGlobalLock();

	if(DEBUG)
	{
		pthread_mutex_lock(&mutex_print);
		printf("\nThread whose address is %x will be executed...\n", node->functionPointer);
		printf("The arguments address is %x ... \n",node->arg);
                pthread_mutex_unlock(&mutex_print);
	}

	void (*fp)(void *) = node->functionPointer;
	void *arg = node->arg;

	#ifdef POWER_SIM 
	pthread_changePowerState(HIGH);  
	#endif 

	(*fp)(arg);
	
	#ifdef POWER_SIM
	pthread_changePowerState(LOW);
	#endif 

	if (DEBUG)
	{
		pthread_mutex_lock(&mutex_print);
		printf("\nThread whose address is %x ended...\n", node->functionPointer);
		printf("The arguments address is %x ... \n",node->arg);
        pthread_mutex_unlock(&mutex_print);
	}

	

	pthread_join_control(&join);

	AcquireGlobalLock();
	pthread_ended --;
	local = pthread_ended;
	ReleaseGlobalLock();

	// the last thread is the responsible for initializing some shared variables
	if (local == 0)
	{
		AcquireGlobalLock();
		join.num_proc = pthread_n_workers;
		pthread_free_for_start = 0;
	    join_out = 0;
		//pthread_numberOfActiveThreads = 0;
		pthread_ended = pthread_n_workers;
		//flag2 = 1;
		ReleaseGlobalLock();
	}
}


int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void (*start_routine)(void *), void *arg)
{
	ThreadNode node;
	initNode (&node, start_routine, arg); 
        
	AcquireGlobalLock();
	enqueue (&tQueue, &node);
	ReleaseGlobalLock();
	
	int size_q = size(&tQueue);
	
	if(size_q == pthread_n_workers)
	{
		AcquireGlobalLock();		
		pthread_created = 1;
		ReleaseGlobalLock();

		pthread_executeThread();

		//#ifdef POWER_SIM 
		//pthread_changePowerState(HIGH);
		//#endif

	}

	return 0;
	
}
/*------------------------ENDING  PTHREAD FUNCTIONS--------------------------------------- */


/*-------------------------STARTING COND FUNCTIONS----------------------------------------- */


int pthread_cond_init (pthread_cond_t *cond, const pthread_condattr_t *attr)
{
	cond->wake = 0;
}

int pthread_cond_wait (pthread_cond_t *cond, pthread_mutex_t *mut)
{
	
	pthread_mutex_unlock(mut);
	while (cond->wake == 0);
	
}

int pthread_cond_broadcast(pthread_cond_t *cond)
{
	cond->wake = 1;
	int i;
	for (i=0; i<=1500; i++);
	cond->wake = 0;
}




/*----------------------------  ENDING COND FUNCTIONS----------------------------------- */


/*------------------------  BARRIER FUNCTIONS---------------------------------- */

void pthread_barrier_init (pthread_barrier_t *barrier, int count)
{
	AcquireGlobalLock();
	barrier->ctrl = 0;
	barrier->counter = barrier-> initial_counter  = count;
	ReleaseGlobalLock();
	
}

int pthread_barrier_wait (pthread_barrier_t *barrier)
{
	int status, ctrl;
	
	AcquireGlobalLock();	
	ctrl = barrier->ctrl;

	if (--barrier->counter == 0)
        {			
		barrier->ctrl = !barrier->ctrl;
                barrier->counter = barrier -> initial_counter;
		ReleaseGlobalLock();
		status = 1;
	}	
        else
        {
		ReleaseGlobalLock();
		while (ctrl == barrier->ctrl);
		status = 1;		
	} 
	
	return status;
	
} // status == 1 -> success

//================================ MPSoCBench malloc ========================================
void *acPthread_malloc(size_t size) { 

	
	pthread_mutex_lock(&mutex_malloc);
	void *ret = (void *) malloc(size); 
        pthread_mutex_unlock(&mutex_malloc);
	
	return ret;
}

//================================ MPSoCBench realloc ========================================
void *acPthread_realloc(void * ptr, size_t size) { 
	pthread_mutex_lock(&mutex_malloc);
	void *ret = (void *) realloc(ptr,size); 
	pthread_mutex_unlock(&mutex_malloc);
	return ret;
}

//================================ MPSoCBench calloc ========================================

void *acPthread_calloc(size_t size1 , size_t size2) { 
	
	pthread_mutex_lock(&mutex_malloc);
	
	void *ret = (void *) calloc(size1,size2); 
        pthread_mutex_unlock(&mutex_malloc);
	return ret;
}



void acPthread_free(void *ptr)
{
	pthread_mutex_lock(&mutex_malloc);
	
	free(ptr); 
	
	pthread_mutex_unlock(&mutex_malloc);
}



void pthread_changePowerState(int state)
{
	//pthread_mutex_lock(&mutex_print);
	//printf("\nem pthread_changePowerState , state %d",state);
	//pthread_mutex_unlock(&mutex_print);
	*dfs = state;
}


void pthread_turnOnProcessors()
{
	*intr_ctrl = ON;
}

void pthread_turnOffProcessors()
{
	*intr_ctrl = OFF;
}