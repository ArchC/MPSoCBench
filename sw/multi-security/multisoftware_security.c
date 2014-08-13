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

//extern unsigned volatile int *lock; 
extern unsigned volatile int *lock;

FILE *fileout_sha;
FILE *fileout_rijndaelenc;
FILE *fileout_rijndaeldec;
FILE *fileout_blowfishenc;
FILE *fileout_blowfishdec;

FILE *filein_sha;
FILE *filein_rijndaelenc;
FILE *filein_rijndaeldec;
FILE *filein_blowfishenc;
FILE *filein_blowfishdec;

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
	 
	printf("\n");
       	printf("--------------------------------------------------------------------\n");
	printf("-------------------------  MPSoCBench  -----------------------------\n");
       	printf("--------------------Running: multi_security ------------------------\n");
       	printf("---------SHA, Rijndael Encoder, Rijndael Decoder, Blowfish----------\n");
	printf("--------------------------------------------------------------------\n");
	printf("\n");

	open_files();

	pthread_mutex_init(&mutex_print,NULL);
	pthread_mutex_init(&mutex_malloc,NULL);

        AcquireGlobalLock();
	barrier_in = 1;
	ReleaseGlobalLock();
     	
	
	main_sha();
	
	pthread_mutex_lock(&mutex_print);
	
	printf("\nSha finished.\n");
	pthread_mutex_unlock(&mutex_print);

  }
  else if (procNumber == 1)
  {
       
	
        while(barrier_in == 0);


	main_rijndael_enc();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nRijndael Encoder finished.\n");
	pthread_mutex_unlock(&mutex_print);
	 
  }
  else if (procNumber == 2)
  {
	
	while(barrier_in == 0);
	

	main_rijndael_dec();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nRijndael Decoder finished.\n");
	pthread_mutex_unlock(&mutex_print);


  }
  else if (procNumber == 3)
  {
	while(barrier_in == 0);

        main_blowfish("E");
	
	pthread_mutex_lock(&mutex_print);
	printf("\nBlowfish Encoder finished.\n");
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

	fclose(fileout_sha);	
	fclose(fileout_rijndaelenc);	
	fclose(fileout_rijndaeldec);	
	fclose(fileout_blowfishenc);

}

void open_files ()
{

	 fileout_sha = fopen("output_sha","w");
	 if (fileout_sha == NULL){
		    printf("Error: fopen() fileout_sha\n");
		    exit(1);
 	 }
	 fileout_rijndaelenc = fopen("output_encoder_rijndael","w");
	 if (fileout_rijndaelenc == NULL){
		    printf("Error: fopen() fileout_rijndaelenc\n");
		    exit(1);
 	 }

	 fileout_rijndaeldec = fopen("output_decoder_rijndael","w");
	 if (fileout_rijndaeldec == NULL){
		    printf("Error: fopen() fileout_rijndaeldec\n");
		    exit(1);
 	 }
	 fileout_blowfishenc = fopen("output_blowfish","w");
	 if (fileout_blowfishenc == NULL){
	     printf("Error: fopen() fileout_blowfishenc\n");
	     exit(1);
	 }
	 

         filein_sha = fopen("input_large.asc","r");
	 if (filein_sha == NULL){
	     printf("Error: fopen() filein_sha\n");
	     exit(1);
	 }

         filein_rijndaelenc = fopen("inputencoder.asc","r");
	 if (filein_rijndaelenc == NULL){
	     printf("Error: fopen() filein_rijndaelenc\n");
	     exit(1);
	 }

	 filein_rijndaeldec = fopen("inputdecoder.enc","r");
	 if (filein_rijndaeldec == NULL){
	     printf("Error: fopen() filein_rijndaeldec\n");
	     exit(1);
	 }
         filein_blowfishenc = fopen("inputencoder_large.asc","r");
	 if (filein_blowfishenc == NULL){
	     printf("Error: fopen() filein_blowfishenc\n");
	     exit(1);
	 }
    
	
	 
}


