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


#ifndef acPthread_H_
#include "../acPthread.h"
#endif


extern unsigned volatile int *lock;


/* OUTPUT FILES */

FILE *fileout_basicmath;
FILE *fileout_qsort;
FILE *fileout_susancorners;
FILE *fileout_dijkstra;


FILE *filein_qsort;
FILE *filein_susancorners;
FILE *filein_dijkstra;


volatile int procCounter = 0;
volatile int barrier_in = 0;
volatile int procsFinished = 0;

extern pthread_mutex_t mutex_print;
extern pthread_mutex_t mutex_malloc;
int NSOFTWARES = 4;
void open_files();
void close_files();

int main(int argc, char *argv[])
{

  

  register int procNumber;
  AcquireGlobalLock();
  procNumber = procCounter++;
  ReleaseGlobalLock();

  if (procNumber == 0){	
	 


	#ifdef POWER_SIM
 	pthread_changePowerState(HIGH);
  	#endif

	printf("\n");
    printf("--------------------------------------------------------------------\n");
	printf("-------------------------  MPSoCBench  -----------------------------\n");
    printf("-----------------Running: multi_network_automotive -----------------\n");
	printf("------------Basicmath, Susan-corners, Dijkstra, Qsort---------------\n");
	printf("--------------------------------------------------------------------\n");
	printf("\n");

	open_files();

	pthread_mutex_init(&mutex_print,NULL);
	pthread_mutex_init(&mutex_malloc,NULL);

        AcquireGlobalLock();
	barrier_in = 1;
	ReleaseGlobalLock();


	main_basicmath();
	pthread_mutex_lock(&mutex_print);
	printf("\nBasicmath finished.\n");
	pthread_mutex_unlock(&mutex_print);



  }
  else if (procNumber == 1)
  {
	

	

	while(barrier_in == 0);
	
	#ifdef POWER_SIM
 	pthread_changePowerState(HIGH);
  	#endif

	main_dijkstra();

	
	pthread_mutex_lock(&mutex_print);
	printf("\nDijkstra finished.\n");
	pthread_mutex_unlock(&mutex_print);



  }
  else if (procNumber == 2)
  {

  	
  	

	while(barrier_in == 0);
	
	if (DVFS) pthread_changePowerState(HIGH);
   
	main_qsort();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nQsort finished.\n");
	pthread_mutex_unlock(&mutex_print);


	


  }
  else if (procNumber == 3)
  {

	while(barrier_in == 0);

	if (DVFS) pthread_changePowerState(HIGH);

       
	main_susancorners();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nSusan corners finished.\n");
	pthread_mutex_unlock(&mutex_print);



  }
  else
  {
	printf("\nERROR!\n");
  }


  AcquireGlobalLock();
  procsFinished++;
  if(procsFinished == NSOFTWARES)
  {
	close_files();
  }
  ReleaseGlobalLock();


  _exit(0);
  return 0;
   
}

void close_files ()
{
	fclose(fileout_basicmath);	
	fclose(fileout_susancorners);
	fclose(fileout_qsort);
	fclose(fileout_dijkstra);	
}


void open_files ()
{

	fileout_basicmath = fopen("output_basicmath","w");
	 if (fileout_basicmath == NULL){
		    printf("Error: fopen() fileout_basicmath\n");
		    exit(1);
	 }
	 fileout_susancorners = fopen("output_susancorners","w");
	 if (fileout_susancorners == NULL){
		    printf("Error: fopen() fileout_susancorners\n");
		    exit(1);
 	 }
	 fileout_qsort = fopen("output_qsort","w");
	 if (fileout_qsort == NULL){
		    printf("Error: fopen() fileout_qsort\n");
		    exit(1);
 	 }
 	 fileout_dijkstra = fopen("output_dijkstra","w");
	 if (fileout_dijkstra == NULL){
		    printf("Error: fopen() fileout_dijkstra\n");
		    exit(1);
 	 }


	 filein_qsort = fopen("input_small.dat","r");
	 if (filein_qsort == NULL){
		    printf("Error: fopen() filein_qsort\n");
		    exit(1);
 	 }
         filein_susancorners = fopen("input_large2.pgm","r");
	 if (filein_susancorners == NULL){
		    printf("Error: fopen() filein_susancorners\n");
		    exit(1);
 	 }
	 filein_dijkstra = fopen("input.dat","r");
	 if (filein_dijkstra == NULL){
		    printf("Error: fopen() filein_dijkstra\n");
		    exit(1);
 	 }


}


