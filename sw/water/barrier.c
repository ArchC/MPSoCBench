#line 156 "c.m4.null"

#line 1 "barrier.C"
/*====================================================================
 * @FILE: .h
 *
 * @DATE: 04/14/2010
/*====================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include "barrier.H"

unsigned volatile int *LOCKER = (unsigned int *)0x20000000;



//Locker
//unsigned int *LOCKER = (unsigned int *)0x20000000;

//#define AcquireGlobalLock	while ((*LOCKER ))

//#define ReleaseGlobalLock	((*LOCKER ) = 0)

/*void AGlobalLock(void)
{
	while ((*LOCKER ));
}

void RGlobalLock(void)
{
	((*LOCKER ) = 0);
}*/

//================================== Barrier ============================================

//Define barrier functions

int barrier_init (barrier_t *barrier, int count)
{
	AGlobalLock();
	//barrier = (barrier_t*) malloc(sizeof(barrier_t));
	barrier->threshold = barrier->counter = count;
	barrier->cycle = 0;
	barrier->valid = BARRIER_VALID;
	RGlobalLock();
	return 0;
}

int barrier_wait (barrier_t *barrier)
{
	int status, cycle;
	
	if (barrier->valid != BARRIER_VALID)
		return 1;

	AGlobalLock();	
	
	cycle = barrier->cycle;

	if (--barrier->counter == 0){			
		barrier->cycle = !barrier->cycle;
		barrier->counter = barrier->threshold;		
		status = -1;
		//printf("Changed %x ==> Counter: %d  Threshold: %d  Cycle: %d\n", barrier, barrier->counter, barrier->threshold, cycle);
		RGlobalLock();
	} else {
		RGlobalLock();
		status = 0;
		//printf("DEC %x ==> Counter: %d  Threshold: %d  Cycle: %d\n", barrier, barrier->counter, barrier->threshold, cycle);
		while (cycle == barrier->cycle);
	} 
	
	// 1 - error, -1 for waker, or 0
	return status;
	
}

//================================ Locker Structure ========================================

int mutex_init(mutex_t *m_lock)
{
	AGlobalLock();
	m_lock->counter = 0;
	m_lock->valid = LOCK_VALID;
	RGlobalLock();
	return 0;
}

int mutex_lock(mutex_t *m_lock)
{
	int status, i;	

	if ( m_lock->valid != LOCK_VALID )
		status = 1;
	
	AGlobalLock();

	//Trying to acquire the lock state
	while (m_lock->counter == 1){
		RGlobalLock();
		for (i = 0; i < 5; i++);
		AGlobalLock();
	}

	//Acquiring the lock state
	m_lock->counter = 1;

	RGlobalLock();

	status = 0;

	return status; //1 - Error / 0 - Success
}

int mutex_unlock(mutex_t *m_lock)
{
	AGlobalLock();
	m_lock->counter = 0;
	RGlobalLock();
}


//================================ Join Point ========================================

int join_init(join_t *m_join, int n_proc)
{
	m_join->num_proc = n_proc;
	m_join->valid = JOIN_VALID;
	return 0;
}

void join_point(join_t *m_join)
{	
	while(m_join->valid != JOIN_VALID);
	AGlobalLock();
	m_join->num_proc--;
	RGlobalLock();
	//printf("JOIN: %d\n",m_join->num_proc);
}

int wait_for_end(join_t *m_join)
{
	while (m_join->num_proc != 0);
	return 0;
}

//================================ Our Malloc ========================================
void *our_malloc(size_t size) { 
	void *ret;	
	AGlobalLock();
	ret = (void *) malloc(size); 
	//ret = (void *) valloc(size);
	RGlobalLock();
	return ret;
}	

//================================ CondWait ========================================

int cond_init(cond_t *cond) {
	cond->value = 0;
	cond->valid = COND_VALID;
	return 0;
}

int cond_wait(cond_t *cond, mutex_t *m_lock){

	while(cond->valid != COND_VALID);	
	
	mutex_unlock(m_lock); 
	printf("cond_wait %d\n", cond->value);
	while (cond->value == 0);

	mutex_lock(m_lock);
	printf("living wait %d\n", cond->value);
	return 0;
}

int cond_broadcast(cond_t *cond){

	while(cond->valid != COND_VALID);

	cond->value = 1;
	//printf("cond_broadcast %d\n", cond->value);
	return 0;
}

