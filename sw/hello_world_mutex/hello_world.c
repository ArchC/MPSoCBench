#include <stdio.h>

#define malloc acPthread_malloc
#define realloc acPthread_realloc
#define free acPthread_free
#define calloc acPthread_calloc

/******************************************************
acPthread
*******************************************************/
#include "../acPthread.h"

extern unsigned volatile int *lock;
extern pthread_mutex_t mutex_print;
extern int pthread_threads_per_software; // number of threads per software 
extern int pthread_n_workers;

unsigned volatile int procCounter;
volatile int barrier_in = 0;
FILE *fileout;
int m_argc;
char *m_argv[3];


extern int pthread_finished;
extern int pthread_created;

int main(int argc, char *argv[])
{
	register int procNumber;

    AcquireGlobalLock();
    printf("ProcNumber: %d\n", procCounter);
    procNumber = procCounter++;
	ReleaseGlobalLock();

	if (procNumber == 0)
 	{	
	
		/* NUMBER OF WORKER THREADS */
		
		pthread_n_workers = NPROC;

	   	m_argc = 3; 
		m_argv[1] = "P";  
		m_argv[2] = "-";    
	       
		pthread_init();

		pthread_mutex_init(&mutex_print, NULL);
		pthread_mutex_lock(&mutex_print);  
		printf("\n");
		printf("\n");
		printf("--------------------------------------------------------------------\n");
		printf("-------------------------  MPSoCBench  -----------------------------\n");
		printf("------------------- Running: hello_world_mutex  --------------------\n");
		printf("--------------- The results will be available in -------------------\n");
		printf("----------------------- the output.dat file ------------------------\n");
		printf("--------------------------------------------------------------------\n");
		printf("\n");
		pthread_mutex_unlock(&mutex_print); 

		AcquireGlobalLock();
		barrier_in = 1;
		ReleaseGlobalLock();
	

		pthread_my_exit();
  	}
  	else
  	{
		pthread_mutex_lock(&mutex_print);  
		printf("SOU O %d\n", procNumber);
		pthread_mutex_unlock(&mutex_print); 
  	}
	_exit(0);  // To avoid cross-compiler exit routine
	return 0; // Never executed, just for compatibility
}
/******************************************************
acPthread
*******************************************************/
