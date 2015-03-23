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

//FILE *fileout_dijkstra;
FILE *fileout_pbm_stringsearch;
FILE *fileout_bmh_stringsearch;
FILE *fileout_fft;
FILE* fileout_adpcmencoder;

FILE* filein_adpcmencoder;

// unsigned char *in;

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
  // printf("\nProcessor %d started.\n", procNumber);
  ReleaseGlobalLock();

 

  if (procNumber == 0){	
	 


 	#ifdef POWER_SIM
 	pthread_changePowerState(HIGH);
  	#endif
   
	printf("\n");
       	printf("--------------------------------------------------------------------\n");
	printf("-------------------------  MPSoCBench  -----------------------------\n");
       	printf("-----------------Running: multi_office_telecomm --------------------\n");
       	printf("---------PBM_stringsearch, BMH_stringsearch, FFT, ADPCM-------------\n");
	printf("--------------------------------------------------------------------\n");
	printf("\n");

	open_files();

	pthread_mutex_init(&mutex_print,NULL);
	pthread_mutex_init(&mutex_malloc,NULL);

        AcquireGlobalLock();
	barrier_in = 1;
	ReleaseGlobalLock();
     	
	
	main_bmh_stringsearch();
	pthread_mutex_lock(&mutex_print);
	printf("\nStringsearch Boyer-Moore-Horspool finished.\n");
	pthread_mutex_unlock(&mutex_print);



  }
  else if (procNumber == 1)
  {
     
    while(barrier_in == 0);

    #ifdef POWER_SIM
 	pthread_changePowerState(HIGH);
  	#endif
	
	main_pbm_stringsearch();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nStringsearch Pratt-Boyer-Moore finished.\n");
	pthread_mutex_unlock(&mutex_print);	 

  }
  else if (procNumber == 2)
  {
	
	while(barrier_in == 0);
	
    #ifdef POWER_SIM
 	pthread_changePowerState(HIGH);
  	#endif

	main_fft();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nFFT finished.\n");
	pthread_mutex_unlock(&mutex_print);


  }
  else if (procNumber == 3)
  {
	while(barrier_in == 0);
	
 		#ifdef POWER_SIM
 		 pthread_changePowerState(HIGH);
  		#endif
    
	main_adpcmencoder();
	pthread_mutex_lock(&mutex_print);
	printf("\nAdpcm Encoder finished.\n");
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
	fclose(fileout_adpcmencoder);
	fclose(fileout_pbm_stringsearch);
	fclose(fileout_fft);
	fclose(fileout_bmh_stringsearch);	

}


void open_files ()
{

      

	 fileout_pbm_stringsearch = fopen("output_pbm_stringsearch","w");
         if (fileout_pbm_stringsearch == NULL){
	     printf("Error: fopen() fileout_pbm_stringsearch\n");
	     exit(1);
	 }

	 fileout_bmh_stringsearch = fopen("output_bmh_stringsearch","w");
         if (fileout_bmh_stringsearch == NULL){
	     printf("Error: fopen() fileout_bmh_stringsearch\n");
	     exit(1);
	 }


	 fileout_fft = fopen("output_fft","w");
	 if (fileout_fft == NULL){
		    printf("Error: fopen() fileout_fft\n");
		    exit(1);
 	 }

         fileout_adpcmencoder = fopen("output_adpcm","w");
	 if (fileout_adpcmencoder == NULL){
	     printf("Error: fopen() fileout_adpcmencoder\n");
	     exit(1);
	 }



         filein_adpcmencoder = fopen("inputencoder.pcm","r");
	 if (filein_adpcmencoder == NULL){
	     printf("Error: fopen() filein_adpcmencoder\n");
	     exit(1);
	 }



	 
}


