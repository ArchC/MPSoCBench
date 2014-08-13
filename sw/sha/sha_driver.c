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


/***************************************************************************
SHA Secure Hash Algorithm				                                
*															Parallel 1.0 Version															
* Authors:           	Yuchen Liang and Syed Muhammad Zeeshan Iqbal
* Supervisor:					Hakan Grahn	 						
* Section:          	Computer Science of BTH,Sweden
* GCC Version:				4.2.4								
* Environment: 				Kraken(Ubuntu4.2.4) with Most 8 Processors 				
* Start Date:       	27th October 2009					
* End Date:         	23th November 2009			
*		
*********************************************************************************/

/* NIST Secure Hash Algorithm */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
//#include <pthread.h>
#include "sha.h"

#define MAX_NO_FILES 64
#define MAX_WORKERS 64
#define PRINT 1

int no_files;  //MAX_NO_FILES;    // this value can be modified but will interfere with the partitioning
int no_workers; 		// this value is an argument (is the same as the number of processors in the platform)
				// it can't be modified directly 
#define malloc acPthread_malloc
#define realloc acPthread_realloc
#define free acPthread_free
#define calloc acPthread_calloc


extern int pthread_finished;
extern int pthread_created;


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

int main(int argc, char *argv[])
{
	register int procNumber;
		
	AcquireGlobalLock();
	procNumber = procCounter++;
	ReleaseGlobalLock();

	if (procNumber == 0)
 	{	
	
		/* NUMBER OF WORKER THREADS */
		
		pthread_n_workers = NPROC;

	   	m_argc = 3; 
		m_argv[1] = "P";  
		m_argv[2] = "-";    
	       
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
        	printf("------------- Running: sha (Secure Hash Algorithm) -----------------\n");
        	printf("--------------- The results will be available in -------------------\n");
        	printf("----------------------- hte output.dat file ------------------------\n");
        	printf("--------------------------------------------------------------------\n");
		printf("\n");
		//printf("There are %d files to digest.\n",no_files);
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
			if(pthread_created == 1) {
				 pthread_executeThread();
			}

		}

		
  	}
	_exit(0);  // To avoid cross-compiler exit routine
	return 0; // Never executed, just for compatibility
}
/******************************************************
acPthread
*******************************************************/

typedef struct Params {
	int s_index;
	int e_index;
	int no;
	char *name;
} parameters;

struct fileData{
	  BYTE* data;
	  int size;
}Tdata[MAX_NO_FILES];


char* in_file_list[]={"in_file1.asc", "in_file2.asc","in_file3.asc","in_file4.asc","in_file5.asc","in_file6.asc","in_file7.asc","in_file8.asc","in_file9.asc","in_file10.asc","in_file11.asc","in_file12.asc","in_file13.asc","in_file14.asc","in_file15.asc","in_file16.asc", "in_file17.asc", "in_file18.asc","in_file19.asc","in_file20.asc","in_file21.asc","in_file22.asc","in_file23.asc","in_file24.asc","in_file25.asc","in_file26.asc","in_file27.asc","in_file28.asc","in_file29.asc","in_file30.asc","in_file31.asc","in_file32.asc", "in_file33.asc", "in_file34.asc","in_file35.asc","in_file36.asc","in_file37.asc","in_file38.asc","in_file39.asc","in_file40.asc","in_file41.asc","in_file42.asc","in_file43.asc","in_file44.asc","in_file45.asc","in_file46.asc","in_file47.asc","in_file48.asc", "in_file49.asc", "in_file50.asc","in_file51.asc","in_file52.asc","in_file53.asc","in_file54.asc","in_file55.asc","in_file56.asc","in_file57.asc","in_file58.asc","in_file59.asc","in_file60.asc","in_file61.asc","in_file62.asc","in_file63.asc","in_file64.asc",NULL};

pthread_attr_t string_attr;
pthread_mutex_t string_mutex;
pthread_t workers[MAX_WORKERS]; 

static int partition_size;
static parameters paramsArr[MAX_WORKERS];


void readFilesData(){
	FILE *fin;
	int index;
	 
	 for ( index=0; index<no_files; index++){
	      if ((fin = fopen(in_file_list[index], "rb")) == NULL) {
					printf("Error opening %s for reading\n", in_file_list[index]);
					}		
	 			// Calculate File Size

		pthread_mutex_lock(&mutex_print);    
	        printf("\nOpening file %s.",in_file_list[index]);
		pthread_mutex_unlock(&mutex_print);


    		fseek(fin, 0, SEEK_END);
		Tdata[index].size = ftell(fin);
		fseek(fin, 0, SEEK_SET);

								
		Tdata[index].data = (BYTE *) calloc(sizeof(BYTE),Tdata[index].size+5);
		
		if (Tdata[index].size != fread(Tdata[index].data, sizeof(BYTE), Tdata[index].size, fin))
			{
					free(Tdata[index].data);
			  		printf("Error: Could not read file!\n");
	   		}
				
	}


		

}


void compute_digest(int index, int n){
	
	   SHA_INFO sha_info;
    
	   sha_stream(&sha_info, Tdata[index].data,Tdata[index].size);
           if (PRINT)
	   {
		pthread_mutex_lock(&mutex_print);    
	        printf("\nWorker number %d: finished.",n);
		fprintf(fileout,"\nWorker number %d: finished.",n);
		sha_print_to_file(&sha_info,fileout);	
		pthread_mutex_unlock(&mutex_print);
	    }
		 
}

void *parallel_SHA_Dig_Calc(void *params){
	
       int index;
       parameters *param=(parameters *)params;
       
       if (PRINT)
       {
		pthread_mutex_lock(&mutex_print);       
		printf("\nWorker %d: calculating digest.",param->no);
		pthread_mutex_unlock(&mutex_print);

       }
       for (index=param->s_index; index<param->e_index; index++){
		compute_digest(index,param->no);
        }
        
}

void init_workers(){
	
	pthread_attr_init(&string_attr);
	pthread_mutex_init(&string_mutex,NULL);
	pthread_attr_setdetachstate(&string_attr,PTHREAD_CREATE_JOINABLE);
	
}

void createWorkers(){
	long index;
	for ( index= 0 ; index<no_workers; index++){
				    
		    pthread_create(&workers[index],&string_attr,(void *)parallel_SHA_Dig_Calc,(void *)&paramsArr[index]);
	}
	
}
void waitForWorkersToFinish(){
	int index;
	    for ( index= 0 ; index<no_workers; index++)
	    	   pthread_join(workers[index],NULL);
	    	   
 }                                               
 
void divide_into_sub_tasks(){

	  int i;
	  int s_index=0;
	  
	  //printf("\nem divide sub_tasks-partition_size->%d\n",partition_size);  
	  for (i=0;i<no_workers;i++) {
			   paramsArr[i].s_index=s_index;
			   paramsArr[i].e_index=s_index+partition_size;
			   paramsArr[i].name="Worker";
			   paramsArr[i].no=i+1;
			   s_index+=partition_size;
			      
	  }
	 ;  
	  // It handles even not of jobs and odd no of workers vice versa.
	   paramsArr[i-1].e_index=paramsArr[i-1].e_index+(no_files%no_workers);
   	  
}

void show_sub_tasks() {

		int i;
			for (i=0;i<no_workers;i++) {
				   
				   pthread_mutex_lock(&mutex_print);
				   fprintf(fileout,"%d   ",paramsArr[i].s_index);
				   fprintf(fileout,"%d   ",paramsArr[i].e_index);
				   fprintf(fileout,"%s ",paramsArr[i].name);
				   fprintf(fileout,"%d\n",paramsArr[i].no);
				   pthread_mutex_unlock(&mutex_print);
		  }
}

void init(){
	  partition_size= no_files/no_workers; //MAX_NO_FILES/no_workers;
	  divide_into_sub_tasks();
	  if (PRINT)
		show_sub_tasks();
}   

void parallel_process(){
	 	//	printf("*******  Digest Calucation:Parallel Process  *******\n");
       
	    init();

	   
	    init_workers();
	    
	    createWorkers();
	    
	    waitForWorkersToFinish();
	    
}

void sequential_process(){
	
	      int index;
       
       printf("*******  Digest Calucation:Sequential Process  *******\n");
       
       for (index=0; index<no_files; index++){
       	if (PRINT){
       	    printf("-------*******************************-------------------\n");
       	    printf("Computing Digest for file: %s\n",in_file_list[index]);
       	  }
       	    
         compute_digest(index,0);
       	   
       	 
      }
	
}


int main0(int argc,char *argv[])
{
	char *token;
      if (argc<2 ||argc>3){
      	  printf("|-----------------------------------------------------------------------|\n");
      	  printf("	Error: Insufficient Parameters.                             \n");
      	  printf("	Maximum Workers are 8. Number of workers should be even!\n");
      	  printf("	Commands to run!                             \n");
      	  printf("	Command Format: OjbectFileName -Sequential(S)/Parallel(P) -Workers!\n");
      	  printf("	Example: sequential SHA: ' ./sha -S   '!                             \n");
      	  printf("	Example: parallel SHA  : ' ./sha -P -2 '!\n");
      	  printf("|---------------------------------------------------------------------- |\n");
      	  exit(0);
      }      	    
      token=argv[1];
      	    
      if (strcmp(token,"S")==0){
       	    readFilesData();
	            sequential_process();
      }
      else if (strcmp(token, "P")==0)
      {
           
	    no_workers= NPROC;
	    no_files = no_workers;

	    printf("\nno_workers: %d\n",no_workers);
	    printf("\nno_files: %d\n",no_files);
    	    readFilesData();
	    parallel_process();	
       }
       else {
	  printf("ERROR: Unknown Parameters!\n");
	}//end-if	  	
      	  	
 	    	
    return(0);
}
