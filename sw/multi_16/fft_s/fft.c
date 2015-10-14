#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef acPthread_H_
#include "../../acPthread.h"
#endif
extern FILE* fileout_fft;
extern pthread_mutex_t mutex_print;

#define MAXSIZE 128 //1024 //1024*2 //1024
#define MAXWAVES  4 

int main_fft(int procNumber) {

	unsigned i,j;
/*	float *RealIn;
	float *ImagIn;
	float *RealOut;
	float *ImagOut;
	float *coeff;
	float *amp;
*/

float RealIn[MAXSIZE];
float ImagIn[MAXSIZE];
float RealOut[MAXSIZE];
float ImagOut[MAXSIZE];
float coeff[MAXSIZE];
float amp[MAXSIZE];

int invfft=0;
 

 /*RealIn=(float*)malloc(sizeof(float)*MAXSIZE);
 ImagIn=(float*)malloc(sizeof(float)*MAXSIZE);
 RealOut=(float*)malloc(sizeof(float)*MAXSIZE);
 ImagOut=(float*)malloc(sizeof(float)*MAXSIZE);
 coeff=(float*)malloc(sizeof(float)*MAXWAVES);
 amp=(float*)malloc(sizeof(float)*MAXWAVES);*/

 /* Makes MAXWAVES waves of random amplitude and period */

	srand(1);
	for(i=0;i<MAXWAVES;i++) 
	{
		coeff[i] = rand()%1000;
		amp[i] = rand()%1000;
	}
 



for(i=0;i<MAXSIZE;i++) 
 {
   /*   RealIn[i]=rand();*/
	 RealIn[i]=0;
	 for(j=0;j<MAXWAVES;j++) 
	 {
		 /* randomly select sin or cos */
		 if (rand()%2)
		 {
		 		RealIn[i]+=coeff[j]*cos(amp[j]*i);
			}
		 else
		 {
		 	RealIn[i]+=coeff[j]*sin(amp[j]*i);
		 }
  	 ImagIn[i]=0;
	 }
 }



 /* regular*/
 fft_float (MAXSIZE,invfft,(float*)RealIn,(float*)ImagIn,(float*)RealOut,(float*)ImagOut);
 
 pthread_mutex_lock(&mutex_print);
 fprintf(fileout_fft,"RealOut:\n");
  
 for (i=0;i<MAXSIZE;i++)
 {
     fprintf(fileout_fft,"%f ", RealOut[i]);
 }
 
 fprintf(fileout_fft,"\nImagOut:\n");

 for (i=0;i<MAXSIZE;i++)
 {
      fprintf(fileout_fft,"%f ", ImagOut[i]);
 }

 fprintf(fileout_fft,"\n");
 pthread_mutex_unlock(&mutex_print);

 /*free(RealIn);
 free(ImagIn);
 free(RealOut);
 free(ImagOut);
 free(coeff);
 free(amp);*/

 return 0;

}
