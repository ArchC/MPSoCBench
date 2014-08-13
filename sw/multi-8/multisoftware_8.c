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



FILE *fileout_pbm_stringsearch;
FILE *fileout_bmh_stringsearch;
FILE *fileout_fft;
FILE* fileout_adpcmencoder;

FILE* filein_adpcmencoder;
// unsigned char *in;

volatile int procCounter = 0;
volatile int barrier_in = 0;
volatile int procsFinished = 0;

int NSOFTWARES = 8;

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
       	printf("-----------------------Running: multi-8 ----------------------------\n");
       	printf("---------SHA, Rijndael Encoder, Rijndael Decoder, Blowfish----------\n");
       	printf("---------PBM_stringsearch, BMH_stringsearch, FFT, ADPCM-------------\n");


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

	main_bmh_stringsearch();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nStringsearch Boyer-Moore-Horspool finished.\n");
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
	

	main_fft();
	
	pthread_mutex_lock(&mutex_print);
	printf("\nFFT finished.\n");
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
	fclose(fileout_rijndaeldec);
	fclose(fileout_blowfishenc);
	fclose(fileout_rijndaelenc);
	fclose(fileout_bmh_stringsearch);
	fclose(fileout_pbm_stringsearch);
	fclose(fileout_fft);
	fclose(fileout_adpcmencoder);

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
	 fileout_blowfishenc = fopen("output_blowfishenc","w");
	 if (fileout_blowfishenc == NULL){
	     printf("Error: fopen() fileout_blowfishenc\n");
	     exit(1);
	 }
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
         fileout_adpcmencoder = fopen("output_encoder_adpcm","w");
	 if (fileout_adpcmencoder == NULL){
	     printf("Error: fopen() fileout_adpcmencoder\n");
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
         filein_adpcmencoder = fopen("inputencoder.pcm","r");
	 if (filein_adpcmencoder == NULL){
	     printf("Error: fopen() filein_adpcmencoder\n");
	     exit(1);
	 }
	

	 
}


