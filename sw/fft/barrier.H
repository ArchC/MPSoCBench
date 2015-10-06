/*====================================================================
 * @FILE: .h
 *
 * @DATE: 04/14/2010
/*====================================================================
*/


#include "../acPthread.h"
extern unsigned volatile int *lock;
extern unsigned volatile int *dvfs;


//Locker
//unsigned volatile int *LOCKER  = (unsigned volatile int *) 0x20000000;
//Startup
//unsigned volatile int *STARTUP = (unsigned volatile int *) 0x21000000;

//================================ Global Locker ========================================


//#define AcquireGlobalLock()	while ((*LOCKER))
//#define ReleaseGlobalLock()	((*LOCKER ) = 0)

//================================== Barrier ============================================
#define BARRIER_VALID  0xdbcafe

#define BARRIER_INITIALIZER(cnt) \
	{PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, \
	BARRIER_VALID, cnt, cnt, 0}

//Structures to describe a barrier

typedef struct barrier_tag_ {
	int valid;
	int threshold;
        int counter;
    	int cycle;
} barrier_t;

//Define barrier functions

int barrier_init (barrier_t *barrier, int count)
{

	int i;
	
	AcquireGlobalLock();
	
	barrier->threshold = barrier->counter = count;
	barrier->cycle = 0;
	barrier->valid = BARRIER_VALID;
	
	ReleaseGlobalLock();
	return 0;
}

int barrier_wait (barrier_t *barrier)
{

	int status, cycle;
	
	if (barrier->valid != BARRIER_VALID)
		return 1;

	AcquireGlobalLock();	
	
	cycle = barrier->cycle;

	if (--barrier->counter == 0){			
		barrier->cycle = !barrier->cycle;
		barrier->counter = barrier->threshold;		
		status = -1;
		//printf("Changed ==> Counter: %d  Threshold: %d  Cycle: %d\n", barrier->counter, barrier->threshold, cycle);
		ReleaseGlobalLock();
	} else {
		ReleaseGlobalLock();
		status = 0;
		//printf("DEC ==> Counter: %d  Threshold: %d  Cycle: %d\n", barrier->counter, barrier->threshold, cycle);
		while (cycle == barrier->cycle);
	} 
	
	// 1 - error, -1 for waker, or 0
	return status;
	
}

//================================ Locker Structure ========================================

#define LOCK_VALID  0xebcafe

//Lock structure
typedef struct mutex_tag {
	int valid;
        int counter;
} mutex_t;

int mutex_init(mutex_t *m_lock)
{



	AcquireGlobalLock();
	
	m_lock->counter = 0;
	m_lock->valid = LOCK_VALID;
	
	ReleaseGlobalLock();
	return 0;
}

int mutex_lock(mutex_t *m_lock)
{
	int status, i;	

	if ( m_lock->valid != LOCK_VALID )
		status = 1;
	
	AcquireGlobalLock();

	//Trying to acquire the lock state
	while (m_lock->counter == 1){
		ReleaseGlobalLock();
		for (i = 0; i < 5; i++);
		AcquireGlobalLock();
	}

	//Acquiring the lock state
	m_lock->counter = 1;

	ReleaseGlobalLock();

	status = 0;

	return status; //1 - Error / 0 - Success
}

int mutex_unlock(mutex_t *m_lock)
{
	AcquireGlobalLock();
	m_lock->counter = 0;
	ReleaseGlobalLock();
}


//================================ Join Point ========================================
#define JOIN_VALID  0xfbcafe

typedef struct join_{
	int valid;
	int num_proc;
} join_t;

join_t myJoinPoint;

int join_init(join_t *m_join, int n_proc)
{

	m_join->num_proc = n_proc;
	m_join->valid = JOIN_VALID;

	
	return 0;
}

void join_point(join_t *m_join)
{	
	int i;
		
	while(m_join->valid != JOIN_VALID);
	AcquireGlobalLock();
	m_join->num_proc--;
	ReleaseGlobalLock();
	
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
	AcquireGlobalLock();
	ret = (void *) malloc(size); 
	ReleaseGlobalLock();
	return ret;
}	



