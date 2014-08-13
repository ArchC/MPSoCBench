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
FILE *fileout_pbm_stringsearch;
FILE *fileout_bmh_stringsearch;
FILE *fileout_fft;
FILE* fileout_adpcmencoder;
FILE* fileout_adpcmdecoder;
FILE *fileout_basicmath_cubic;
FILE *fileout_basicmath_conversion;
FILE *fileout_qsort;
FILE *fileout_susancorners;
FILE *fileout_dijkstra;
FILE *fileout_bitcount;

FILE *filein_sha;
FILE *filein_rijndaelenc;
FILE *filein_rijndaeldec;
FILE *filein_blowfishenc;
FILE *filein_blowfishdec;
FILE* filein_adpcmencoder;
FILE* filein_adpcmdecoder;
FILE *filein_qsort;
FILE *filein_susancorners;
FILE *filein_dijkstra;


// unsigned char *in;

volatile int procCounter = 0;
volatile int procsFinished = 0;
volatile int barrier_in = 0;
int NSOFTWARES = 16;

extern pthread_mutex_t mutex_print;
extern pthread_mutex_t mutex_malloc;

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
       	printf("----------------------Running: multi-16 ----------------------------\n");
       	printf("--------ADPCM-encoder, ADPCM-decoder, Basicmath-conversion, --------\n");
        printf("---Basicmath-cubic, Bitcount, Blowfish-encoder, Blowfish-decoder ---\n"); 
	printf("--- BMH-Stringsearch, Dijkstra, FFT, PBM_stringsearch, QSort -------\n");
        printf("------ Rijndael-encoder, Rijndael-decoder, SHA, Susan-corners ------\n");
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
  else if (procNumber == 4)
  {

	while(barrier_in == 0);

        main_blowfish("D");
	
	pthread_mutex_lock(&mutex_print);
	printf("\nBlowfish Decoder finished.\n");
	pthread_mutex_unlock(&mutex_print);

	


  }
  else if (procNumber == 5)
  {
     
        while(barrier_in == 0);

	main_pbm_stringsearch();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nStringsearch Pratt-Boyer-Moore finished.\n");
	pthread_mutex_unlock(&mutex_print);	 
	
	
  }
  else if (procNumber == 6)
  {
	while(barrier_in == 0);

	main_bmh_stringsearch();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nStringsearch Boyer-Moore-Horspool finished.\n");
	pthread_mutex_unlock(&mutex_print);

	

	
  }
  else if (procNumber == 7)
  {
	while(barrier_in == 0);
	
	main_adpcmencoder();
	pthread_mutex_lock(&mutex_print);
	printf("\nAdpcm Encoder finished.\n");
	pthread_mutex_unlock(&mutex_print);

	
  }

  else if (procNumber == 8)
  {
	
	while(barrier_in == 0);
	
	main_basicmath_cubic();

	pthread_mutex_lock(&mutex_print);
	printf("\nBasicmath_cubic finished.\n");
	pthread_mutex_unlock(&mutex_print);

	

  }


else if (procNumber == 9)
  {
	
	while(barrier_in == 0);
	
	main_dijkstra();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nDijkstra finished.\n");
	pthread_mutex_unlock(&mutex_print);

	

  }
  else if (procNumber == 10)
  {
	while(barrier_in == 0);
	
	main_qsort();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nQsort finished.\n");
	pthread_mutex_unlock(&mutex_print);

	
	
  }
  else if (procNumber == 11)
  {

	while(barrier_in == 0);

	main_susancorners();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nSusan-corners finished.\n");
	pthread_mutex_unlock(&mutex_print);

	

  }
  else if (procNumber == 12)
  {
	while(barrier_in == 0);
	
	main_fft();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nFFT finished.\n");
	pthread_mutex_unlock(&mutex_print);

	
  }


  else if (procNumber == 13)
  {
	while(barrier_in == 0);
	
	main_adpcmdecoder();

	pthread_mutex_lock(&mutex_print);
	printf("\nAdpcm Decoder finished.\n");
	pthread_mutex_unlock(&mutex_print);

	

  }
  else if (procNumber == 14)
  {
	while(barrier_in == 0);
	
	main_basicmath_conversion();

	pthread_mutex_lock(&mutex_print);
	printf("\nBasicmath_conversion finished.\n");
	pthread_mutex_unlock(&mutex_print);

	
  }
  else if (procNumber == 15)
  {
	while(barrier_in == 0);
	
	main_bitcount();

	pthread_mutex_lock(&mutex_print);
	printf("\nBitcount finished.\n");
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
	fclose(fileout_blowfishdec);
	fclose(fileout_pbm_stringsearch);
	fclose(fileout_bmh_stringsearch);
	fclose(fileout_fft);
	fclose(fileout_adpcmencoder);
	fclose(fileout_adpcmdecoder);
	fclose(fileout_basicmath_cubic);
	fclose(fileout_basicmath_conversion);
	fclose(fileout_susancorners);
	fclose(fileout_qsort);
	fclose(fileout_dijkstra);
	fclose(fileout_bitcount);
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
		    printf("Error: fopen() output_encoder_rijndael\n");
		    exit(1);
 	 }

	 fileout_rijndaeldec = fopen("output_decoder_rijndael","w");
	 if (fileout_rijndaeldec == NULL){
		    printf("Error: fopen() output_encoder_rijndael\n");
		    exit(1);
 	 }
	 fileout_blowfishenc = fopen("output_blowfishenc","w");
	 if (fileout_blowfishenc == NULL){
	     printf("Error: fopen() output_blowfishenc\n");
	     exit(1);
	 }
	 fileout_blowfishdec = fopen("output_blowfishdec","w");
	 if (fileout_blowfishdec == NULL){
	     printf("Error: fopen() output_blowfishdec\n");
	     exit(1);
	 }

	 fileout_pbm_stringsearch = fopen("output_pbm_stringsearch","w");
         if (fileout_pbm_stringsearch == NULL){
	     printf("Error: fopen() output_pbm_stringsearch\n");
	     exit(1);
	 }

	 fileout_bmh_stringsearch = fopen("output_bmh_stringsearch","w");
         if (fileout_bmh_stringsearch == NULL){
	     printf("Error: fopen() output_bmh_stringsearch\n");
	     exit(1);
	 }


	 fileout_fft = fopen("output_fft","w");
	 if (fileout_fft == NULL){
		    printf("Error: fopen() output_fft\n");
		    exit(1);
 	 }

         fileout_adpcmencoder = fopen("output_encoder_adpcm","w");
	 if (fileout_adpcmencoder == NULL){
	     printf("Error: fopen() output_encoder_adpcm\n");
	     exit(1);
	 }
	 fileout_adpcmdecoder = fopen("output_decoder_adpcm","w");
	 if (fileout_adpcmdecoder == NULL){
	     printf("Error: fopen() output_decoder_adpcm\n");
	     exit(1);
	 }
        
	 fileout_basicmath_cubic = fopen("output_basicmath_cubic","w");
	 if (fileout_basicmath_cubic == NULL){
		    printf("Error: fopen() output_basicmath_cubic\n");
		    exit(1);
	 }
	
	 fileout_basicmath_conversion = fopen("output_basicmath_conversion","w");
	 if (fileout_basicmath_conversion == NULL){
		    printf("Error: fopen() fileout_basicmath_conversion\n");
		    exit(1);
	 }
	 fileout_susancorners = fopen("output_susancorners","w");
	 if (fileout_susancorners == NULL){
		    printf("Error: fopen() fileout_susancorners\n");
		    exit(1);
 	 }
	 /*fileout_susanedges = fopen("../../sw/multi-16/susanedges_s/output.pgm","w");
	 if (fileout_susanedges == NULL){
		    printf("Error: fopen() fileout_susanedges\n");
		    exit(1);
 	 }*/
	 fileout_qsort = fopen("output_qsort","w");
	 if (fileout_qsort == NULL){
		    printf("Error: fopen() output_qsort\n");
		    exit(1);
 	 }
 	 fileout_dijkstra = fopen("output_dijkstra","w");
	 if (fileout_dijkstra == NULL){
		    printf("Error: fopen() output_dijkstra\n");
		    exit(1);
 	 }
	 fileout_bitcount = fopen("output_bitcount","w");
	 if (fileout_bitcount == NULL){
		    printf("Error: fopen() fileout_bitcount\n");
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

	 /*filein_susanedges = fopen("../../sw/multi-16/susanedges_s/input_small.pgm","r");
	 if (filein_susanedges == NULL){
		    printf("Error: fopen() filein_susanedges\n");
		    exit(1);
 	 }*/

	filein_dijkstra = fopen("input.dat","r");
	 if (filein_dijkstra == NULL){
		    printf("Error: fopen() filein_dijkstra\n");
		    exit(1);
 	 }
	filein_sha = fopen("input_small.asc","r");
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


         filein_blowfishenc = fopen("inputencoder_bl.asc","r");
	 if (filein_blowfishenc == NULL){
	     printf("Error: fopen() filein_blowfishenc\n");
	     exit(1);
	 }
         filein_blowfishdec = fopen("inputdecoder_bl.enc","r");
	 if (filein_blowfishdec == NULL){
	     printf("Error: fopen() filein_blowfishdec\n");
	     exit(1);
	 }

	 filein_adpcmencoder = fopen("inputencoder.pcm","r");
	 if (filein_adpcmencoder == NULL){
	     printf("Error: fopen() filein_adpcmencoder\n");
	     exit(1);
	 }

	 filein_adpcmdecoder = fopen("inputdecoder.adpcm","r");
	 if (filein_adpcmdecoder == NULL){
	     printf("Error: fopen() filein_adpcmdecoder\n");
	     exit(1);
	 }



	
	 
}


