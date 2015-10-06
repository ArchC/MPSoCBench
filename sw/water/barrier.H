//================================ Global Locker ========================================

#include "../acPthread.h"


extern unsigned volatile int *lock;

extern unsigned volatile int *dvfs;



//#define AGlobalLock()	while (*LOCKER)
//#define RGlobalLock()	((*LOCKER)=0)

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

int barrier_init (barrier_t *barrier, int count);

int barrier_wait (barrier_t *barrier);

//================================ Locker Structure ========================================

#define LOCK_VALID  0xebcafe

//Lock structure
typedef struct mutex_tag {
	int valid;
        int counter;
} mutex_t;

int mutex_init(mutex_t *m_lock);

int mutex_lock(mutex_t *m_lock);

int mutex_unlock(mutex_t *m_lock);

//================================ Join Point ========================================
#define JOIN_VALID  0xfbcafe

typedef struct join_{
	int valid;
	int num_proc;
} join_t;

join_t myJoinPoint;

int join_init(join_t *m_join, int n_proc);

void join_point(join_t *m_join);

int wait_for_end(join_t *m_join);

//================================ Our Malloc ========================================
void *our_malloc(size_t size);

//================================ CondWait ========================================
#define COND_VALID  0xabcafe

typedef struct cond_{
	int valid;
	int value;
} cond_t;

int cond_init(cond_t *cond);

int cond_wait(cond_t *cond, mutex_t *m_lock);

int cond_broadcast(cond_t *cond);


