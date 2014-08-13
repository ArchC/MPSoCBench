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

#include <stdio.h>
// #include <stdlib.h>


#include "acPthread_p.h"

#define NSOFTWARES 4

/* OUTPUT FILES */
FILE *fileout_sha;
FILE *fileout_stringsearch;
FILE *fileout_dijkstra;
FILE *fileout_basicmath;

/* INPUT FILES */
FILE *filein1_stringsearch;
FILE *filein2_stringsearch;
FILE *filein_dijkstra;


FILE *filein1_sha;
FILE *filein2_sha;
FILE *filein3_sha;
FILE *filein4_sha;
FILE *filein5_sha;
FILE *filein6_sha;
FILE *filein7_sha;
FILE *filein8_sha;
FILE *filein9_sha;
FILE *filein10_sha;
FILE *filein11_sha;
FILE *filein12_sha;
FILE *filein13_sha;
FILE *filein14_sha;
FILE *filein15_sha;
FILE *filein16_sha;

extern unsigned volatile int *lock;
unsigned volatile int procCounter;


volatile int barrier_in = 0;

extern pthread_mutex_t mutex_print;
extern pthread_join_t join;
extern int pthread_n_softwares;
extern int pthread_threads_per_software; // number of threads per software 
extern int pthread_n_workers;

extern int pthread_free_for_start;
extern int pthread_numberOfActiveThreads;


void open_files ()
{
         

 	 fileout_stringsearch = fopen("output_stringsearch","w");
	 if (fileout_stringsearch == NULL){
	     printf("Error: fopen() fileout_stringsearch\n");
	     exit(0);
	 }

	 fileout_dijkstra = fopen("output_dijkstra","w");
	 if (fileout_dijkstra == NULL){
		    printf("Error: fopen() fileout_dijkstra\n");
		    exit(0);
	 }

	 fileout_basicmath = fopen("output_basicmath","w");
	 if (fileout_basicmath == NULL){
		    printf("Error: fopen() fileout_basicmath\n");
		    exit(0);
	 }
	 
         fileout_sha = fopen("output_sha","w");
	 if (fileout_sha == NULL){
		    printf("Error: fopen() fileout_sha\n");
		    exit(0);
 	 }
	

	 filein1_stringsearch = fopen("SearchString01.txt","r");
	 if (filein1_stringsearch == NULL){
	     printf("Error: fopen() filein1_stringsearch\n");
	     exit(0);
	 }


	 filein2_stringsearch = fopen("testpattern5.txt","r");
	 if (filein2_stringsearch == NULL){
	     printf("Error: fopen() filein1_stringsearch\n");
	     exit(0);

	 }

	 filein_dijkstra = fopen("input_small.dat","r");
	 if (filein_dijkstra == NULL){
	     printf("Error: fopen() filein1_stringsearch\n");
	     exit(0);
	 }

    

}

int main(int argc, char *argv[])
{

  register int procNumber;
  AcquireGlobalLock();
  procNumber = procCounter++;
  ReleaseGlobalLock();

  if (procNumber == 0){	
	 
	pthread_n_workers = NPROC;
	pthread_n_softwares = NSOFTWARES;
	pthread_threads_per_software = pthread_n_workers/pthread_n_softwares;
	
	 

        pthread_init(); 

	pthread_mutex_init(&mutex_print, NULL);
	pthread_mutex_lock(&mutex_print);	
	printf("\n");
        printf("\n");
       	printf("--------------------------------------------------------------------\n");
	printf("-------------------------  MPSoCBench  -----------------------------\n");
       	printf("----------------Running: multisoftware platform --------------------\n");
       	printf("--------------sha, stringsearch, dijkstra, basicmath ---------------\n");
       	printf("--------------- The results will be available in -------------------\n");
       	printf("--------------------------------------------------------------------\n");
       	printf("-----------  sw/multi_parallel/sha_multi/output.dat ----------------\n");
       	printf("-----  sw/multi_parallel/stringsearch_multi/testdata/output.dat ----\n");
       	printf("--------  sw/multi_parallel/dijkstra_multi/output.dat --------------\n");
       	printf("---------- sw/multi_parallel/basicmath_multi/output.dat ------------\n");
       	printf("--------------------------------------------------------------------\n");
	printf("---------------------- %d processadores ----------------------------\n", pthread_n_workers);
	printf("------------------------ %d softwares ------------------------------\n", pthread_n_softwares);
	printf("------------ ------- %d threads per software -----------------------\n", pthread_threads_per_software);
       	printf("--------------------------------------------------------------------\n");
        pthread_mutex_unlock(&mutex_print);

	open_files ();

	pthread_mutex_lock(&mutex_print);  
	printf("---------------- Start running: stringsearch -----------------------\n");
	pthread_mutex_unlock(&mutex_print);  

	AcquireGlobalLock();
	barrier_in = 1;
	ReleaseGlobalLock();

	
	main_stringsearch();
		
     
  }
  else if (procNumber == 1)
  {

	while(barrier_in == 0);

	pthread_mutex_lock(&mutex_print);  
	printf("------------------- Start running: dijkstra ------------------------\n");
	pthread_mutex_unlock(&mutex_print);  

	main_dijkstra();
  }
  else if (procNumber == 2)
  {

	while(barrier_in == 0);

	pthread_mutex_lock(&mutex_print);		
      	printf("------------------- Start running: basicmath -----------------------\n");
 	pthread_mutex_unlock(&mutex_print);		

	main_basicmath ();		

  }
  else if (procNumber == 3)
  {

	while(barrier_in == 0);

	pthread_mutex_lock(&mutex_print);  
        printf("--------------------- Start running: sha --------------------------\n");
	pthread_mutex_unlock(&mutex_print); 
	main_sha();  
  }
  else
  {

	while(barrier_in == 0);

	pthread_executeThread();
  }


  _exit(0);
  return 0;
  
  

  
}


