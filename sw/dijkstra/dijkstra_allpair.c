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



/********************************************************************************
* ORIGINAL VERSION OF DIJKSTRA						      			       				   
* Dijkstra				                                
* Parallel 1.0 Version with Pthread
*															
* Authors:           	Yuchen Liang and Syed Muhammad Zeeshan Iqbal
* Supervisor:		Hakan Grahn	 						
* Section:          	Computer Science of BTH,Sweden
* GCC Version:		4.2.4								
* Environment: 		Kraken(Ubuntu4.2.4) with Most 8 Processors 				
* Start Date:       	15th October 2009					
* End Date:         	3rd November 2009			
*		
*********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
//#include <pthread.h>
#include <sys/types.h>

#define NUM_NODES                          16		//16 for small input; 160 for large input; 30 for medium input;
#define NONE                               9999		//Maximum
#define MAXPROCESSORS			   64		//The amount of processor

int PROCESSORS;

/******************************************************
acPthread
*******************************************************/
#include "../acPthread.h"

extern unsigned volatile int *lock;
extern pthread_mutex_t mutex_print;
extern int pthread_threads_per_software; // number of threads per software 
extern int pthread_n_workers;

#define malloc acPthread_malloc
#define realloc acPthread_realloc
#define free acPthread_free
#define calloc acPthread_calloc

unsigned volatile int procCounter;
volatile int barrier_in = 0;
FILE *fileout;
int m_argc;
char *m_argv[3];

extern int pthread_finished;
extern int pthread_created;
//extern int flag;


int main(int argc, char *argv[])
{

	register int procNumber;
	
	AcquireGlobalLock();
	procNumber = procCounter++;
	ReleaseGlobalLock();

	if (procNumber == 0)
	{
	
 		pthread_n_workers = NPROC;
        printf("\npthread_n_workers:%d",pthread_n_workers);

		PROCESSORS = pthread_n_workers;	

		m_argc = 2;
		m_argv[1] = "../../sw/dijkstra/input_small.dat";

		/* OUTPUT FILE */
		fileout = fopen("output","w");
		if (fileout == NULL){
			    printf("Error: fopen()\n");
			    exit(1);
		}

		pthread_init();
			
		pthread_mutex_init(&mutex_print, NULL);
		pthread_mutex_lock(&mutex_print);  
		printf("\n");
		printf("\n");
		printf("--------------------------------------------------------------------\n");
		printf("-------------------------  MPSoCBench  -----------------------------\n");
		printf("---------------------- Running: dijkstra ---------------------------\n");
		printf("--------------- The results will be available in -------------------\n");
		printf("--------------------- the output.dat file --------------------------\n");
		printf("--------------------------------------------------------------------\n");
		printf("\n");
		pthread_mutex_unlock(&mutex_print);  

		AcquireGlobalLock();
		barrier_in = 1;
		ReleaseGlobalLock();
		
		main0(m_argc,m_argv);

		pthread_my_exit();
		fclose(fileout);
	
  	}
  	else
  	{
		while(barrier_in == 0);
		
		while(pthread_finished == 0)
		{
			if(pthread_created == 1)pthread_executeThread();

		}

  	
	}
	_exit(0);
	//_exit(0);  // To avoid cross-compiler exit routine
	return 0; // Never executed, just for compatibility
}


/******************************************************
acPthread
*******************************************************/



/* define a thread pool */
struct _THREADPOOL{
	int id;			/* thread id */
	pthread_t thread;		/* thread itself */
};
typedef struct _THREADPOOL THREADPOOL;
THREADPOOL threadPool[MAXPROCESSORS]; 

struct _BARRIER{
	int count;
	pthread_mutex_t barrier_mutex;
	pthread_cond_t barrier_cond;
};
typedef struct _BARRIER BARRIER;
BARRIER myBarrier;

struct _THREADARGS{
	int id;			/* thread id */	
};
typedef struct _THREADARGS THREADARGS;


struct _NODE{
	int iDist;
	int iPrev;
	int iCatched;
};
typedef struct _NODE NODE;


struct _QITEM{
	int iNode;
	int iDist;
	int iPrev;
	struct _QITEM *qNext;
};
typedef struct _QITEM QITEM;


struct _UVERTEX{
	int iPID;
	int iNID;
	int iDist;
};
typedef struct _UVERTEX UVERTEX;

int nodes_tasks[NUM_NODES*(NUM_NODES-1)/2][2];
int tasks[MAXPROCESSORS][2];

QITEM *qHead = NULL;
                  
int AdjMatrix[NUM_NODES][NUM_NODES];

int g_qCount[MAXPROCESSORS];
int chStart,chEnd;
NODE rgnNodes[MAXPROCESSORS][NUM_NODES];
UVERTEX uVertex[MAXPROCESSORS];
int globalMiniCost[MAXPROCESSORS];
int us[MAXPROCESSORS];
int iPrev, iNode;

/* Forward declaring functions*/
void print_path(NODE*,int);
void d_enqueue(int,int,int);
void d_dequeue(int*,int*,int*);
int qcount (int);
void* startWorking(void*);
void startThreads(void);
int dijkstra(int);
void printResult(int,int,int);

void print_path(NODE *rgnNodes, int chNode){
	if ((rgnNodes+chNode)->iPrev != NONE){
		print_path(rgnNodes, (rgnNodes+chNode)->iPrev);
	}
	fprintf (fileout," %d", chNode+1);
	//fflush(stdout);
}

void printResult(int myID,int chStart, int chEnd){

	pthread_mutex_lock(&mutex_print);
        fprintf(fileout,"From %d to %d, shortest path is %d in cost. ", chStart+1,chEnd+1, rgnNodes[myID][chEnd].iDist);
        fprintf(fileout,"Path is:  %d",chStart+1);
        print_path(rgnNodes[myID], chEnd);
        fprintf(fileout,"\n");
	pthread_mutex_unlock(&mutex_print);
}


void d_enqueue (int iNode, int iDist, int iPrev){
	QITEM *qNew = (QITEM *) malloc(sizeof(QITEM));
	QITEM *qLast = qHead;  
	if (!qNew) {
      	fprintf(stderr, "Out of memory.\n");
      	exit(1);
	}
	qNew->iNode = iNode;
	qNew->iDist = iDist;
	qNew->iPrev = iPrev;
	qNew->qNext = NULL;  
	if (!qLast) {
		qHead = qNew;
	}else{
		while (qLast->qNext){ 
			qLast = qLast->qNext;
		}
		qLast->qNext = qNew;
	}
	//g_qCount++;
}


void d_dequeue (int *piNode, int *piDist, int *piPrev){
	QITEM *qKill = qHead;
	if (qHead){
      	*piNode = qHead->iNode;
      	*piDist = qHead->iDist;
      	*piPrev = qHead->iPrev;
      	qHead = qHead->qNext;
     		free(qKill);
		//g_qCount--;
	}
}


int qcount (int myID){
	return(g_qCount[myID]);
}


/*
// Intilize barrier 
void startBarrier(BARRIER* myBarrier){
	pthread_mutex_init(&(myBarrier->barrier_mutex), NULL);
	pthread_cond_init(&(myBarrier->barrier_cond), NULL);
	myBarrier->count = 0;
}


// Actuate barrier 
void actuateBarrier(BARRIER* myBarrier){
	pthread_mutex_lock(&(myBarrier->barrier_mutex));
	myBarrier->count++;
	if (myBarrier->count!=PROCESSORS) {
		pthread_cond_wait(&(myBarrier->barrier_cond), &(myBarrier->barrier_mutex));
	}
	else{
		myBarrier->count=0;
		pthread_cond_broadcast(&(myBarrier->barrier_cond));
	}
	pthread_mutex_unlock(&(myBarrier->barrier_mutex));
}

*/
/* Initialize a thread pool and create the threads */

void startThreads(void){


	int i,pc;
	for(i=0;i<PROCESSORS;i++){
		pthread_t thread;
		THREADARGS* threadArgs = (THREADARGS*)malloc(sizeof(THREADARGS));
		threadArgs->id = i;
		pc = pthread_create(&thread,NULL,(void *)startWorking,(void*)threadArgs);
		if(pc != 0){
			printf("Fail to create threads!");
			exit(0);
		}else{
			threadPool[i].id = i;
			threadPool[i].thread = thread;
		}
  	}

	
  	for(i=0;i<PROCESSORS;i++){
      	pthread_join(threadPool[i].thread,NULL);
  	}
}


void* startWorking(void* threadArgs){ //
 	int i,j;
 	THREADARGS* args = (THREADARGS*)threadArgs;
	int myID = args->id;	
	dijkstra(myID);
	return NULL;
}


int dijkstra(int myID) {
	int x,i,v,iDist;
	int chStart, chEnd;
	int u =-1;

	for(x=tasks[myID][0]; x<tasks[myID][1]; x++){
		chStart = nodes_tasks[x][0];	//Start node
		chEnd = nodes_tasks[x][1];	//End node
		u=-1;
		//Initialize and clear	
		uVertex[myID].iDist=NONE;
		uVertex[myID].iPID=myID;
		uVertex[myID].iNID=NONE;
		g_qCount[myID] = 0;
		u=-1;
		for (v=0; v<NUM_NODES; v++) {
			rgnNodes[myID][v].iDist =  AdjMatrix[chStart][v];
			rgnNodes[myID][v].iPrev = NONE;
			rgnNodes[myID][v].iCatched = 0;
		}	
		//Start working
     		while (qcount(myID) < NUM_NODES-1){
			for (i=0; i<NUM_NODES; i++) {
				if(rgnNodes[myID][i].iCatched==0 && rgnNodes[myID][i].iDist<uVertex[myID].iDist && rgnNodes[myID][i].iDist!=0){
					uVertex[myID].iDist=rgnNodes[myID][i].iDist;
					uVertex[myID].iNID=i;
				}		
			}				
			globalMiniCost[myID]=NONE;
			if(globalMiniCost[myID]>uVertex[myID].iDist){
				globalMiniCost[myID] = uVertex[myID].iDist;
				u=uVertex[myID].iNID;
				g_qCount[myID]++;
			}	
			for (v=0; v<NUM_NODES; v++) {
				if(v==u){
					rgnNodes[myID][v].iCatched = 1;
					continue;
				}
				if((rgnNodes[myID][v].iCatched==0 && rgnNodes[myID][v].iDist>(rgnNodes[myID][u].iDist+AdjMatrix[u][v]))){	
					rgnNodes[myID][v].iDist=rgnNodes[myID][u].iDist+AdjMatrix[u][v];	
					rgnNodes[myID][v].iPrev = u; 		
				}
			}
			uVertex[myID].iDist = NONE;	//Reset 
		}
		printResult(myID,chStart,chEnd);
	}  
}


void divide_task_group(int task){ // 2m-1,2g, 4m-1,2,4g, 6m-1,2,3,6g, 8m-1,2,4,8g
	int i;
	for(i=0;i<PROCESSORS;i++){
			tasks[i][0] = task/PROCESSORS* (i);
			tasks[i][1] = task/PROCESSORS* (i+1) + (i+1==PROCESSORS&task%PROCESSORS!=0?task%PROCESSORS:0);
	}
}


int main0(int argc, char *argv[]) {
	
	

        int i,j,k;
	if (argc<2) {
		fprintf(stderr, "Usage: dijkstra <filename>\n");
		fprintf(stderr, "Only supports matrix size is #define'd.\n");
	}
	//Open the adjacency matrix file
	FILE *fp;
	fp = fopen (argv[1],"r");
	/*Step 1: geting the working vertexs and assigning values*/
  	for (i=0;i<NUM_NODES;i++) {
    		for (j=0;j<NUM_NODES;j++) {
     			fscanf(fp,"%d",&k);
			AdjMatrix[i][j]= k;
    		}
  	}
	fclose(fp);
	int tasks = NUM_NODES*(NUM_NODES-1)/2;
	int x=0;
	for(i=0;i<NUM_NODES-1;i++){ //small:15; large:159
		for(j=i+1;j<NUM_NODES;j++){	//small:16; large:160
			nodes_tasks[x][0] = i;
			nodes_tasks[x][1] = j;
			x++;
		}
	}
	
	divide_task_group(tasks);
	startThreads();			/* Start pthreads */	

	

	return 0;
}
