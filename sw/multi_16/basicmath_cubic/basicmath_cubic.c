#include "snipmath.h"
#include <math.h>
#include <stdio.h>
#ifndef acPthread_H_
#include "../../acPthread.h"
#endif


#define SMALL 1
extern FILE *fileout_basicmath_cubic;
extern pthread_mutex_t mutex_print;

int main_basicmath_cubic(void)
{
  double  a1 = 1.0, b1 = -10.5, c1 = 32.0, d1 = -30.0;
  double  a2 = 1.0, b2 = -4.5, c2 = 17.0, d2 = -30.0;
  double  a3 = 1.0, b3 = -3.5, c3 = 22.0, d3 = -31.0;
  double  a4 = 1.0, b4 = -13.7, c4 = 1.0, d4 = -35.0;
  double  x[3];
  double X;
  int     solutions;
  int i;
  unsigned long l = 0x3fed0169L;
  struct int_sqrt q;
  long n = 0;

  /* solve soem cubic functions */
  pthread_mutex_lock(&mutex_print);
  fprintf(fileout_basicmath_cubic,"********* CUBIC FUNCTIONS ***********\n");
  /* should get 3 solutions: 2, 6 & 2.5   */
  pthread_mutex_unlock(&mutex_print);

  SolveCubic(a1, b1, c1, d1, &solutions, x);  

  pthread_mutex_lock(&mutex_print);
  for(i=0;i<solutions;i++)
    fprintf(fileout_basicmath_cubic," %f",x[i]);
  fprintf(fileout_basicmath_cubic,"\n");
  pthread_mutex_unlock(&mutex_print);

  /* should get 1 solution: 2.5           */
  SolveCubic(a2, b2, c2, d2, &solutions, x);  

  pthread_mutex_lock(&mutex_print);
  for(i=0;i<solutions;i++)
    fprintf(fileout_basicmath_cubic," %f",x[i]);
  fprintf(fileout_basicmath_cubic,"\n");
  pthread_mutex_unlock(&mutex_print);

  SolveCubic(a3, b3, c3, d3, &solutions, x);

  pthread_mutex_lock(&mutex_print);
  for(i=0;i<solutions;i++)
    fprintf(fileout_basicmath_cubic," %f",x[i]);
  fprintf(fileout_basicmath_cubic,"\n");
  pthread_mutex_unlock(&mutex_print);

  SolveCubic(a4, b4, c4, d4, &solutions, x);

  pthread_mutex_lock(&mutex_print);
  for(i=0;i<solutions;i++)
    fprintf(fileout_basicmath_cubic," %f",x[i]);
  fprintf(fileout_basicmath_cubic,"\n");
  pthread_mutex_unlock(&mutex_print);

/*

  if(SMALL)
  { 
  for(a1=1;a1<5;a1+=1) {
    for(b1=10;b1>0;b1-=1.0) {
      for(c1=10;c1<15;c1+=1.0) {
	for(d1=-1;d1>-4;d1=d1-0.25) {
	  SolveCubic(a1, b1, c1, d1, &solutions, x);  
          pthread_mutex_lock(&mutex_print);
	  for(i=0;i<solutions;i++)
	    fprintf(fileout_basicmath_cubic," %f",x[i]);
	  fprintf(fileout_basicmath_cubic,"\n");
          pthread_mutex_unlock(&mutex_print); 
	}
      }
    }
   }
  }
  else
  { 
  for(a1=1;a1<5;a1+=0.1) {
    for(b1=10;b1>0;b1-=0.1) {
      for(c1=10;c1<15;c1+=0.1) {
	for(d1=-1;d1>-4;d1=d1-0.25) {
	  SolveCubic(a1, b1, c1, d1, &solutions, x);  
          pthread_mutex_lock(&mutex_print);
	  for(i=0;i<solutions;i++)
	    fprintf(fileout_basicmath_cubic," %f",x[i]);
	  fprintf(fileout_basicmath_cubic,"\n");
          pthread_mutex_unlock(&mutex_print); 
	}
      }
    }
   }
  }

  
  pthread_mutex_lock(&mutex_print);
  fprintf(fileout_basicmath_cubic,"********* INTEGER SQR ROOTS ***********\n");
  
  for (i = 0; i < 501; ++i)
    {
      usqrt(i, &q);
			
     
     fprintf(fileout_basicmath_cubic,"sqrt(%3d) = %2d\n",
	     i, q.sqrt);
    }
  pthread_mutex_unlock(&mutex_print);
  usqrt(l, &q);
  

 
  pthread_mutex_lock(&mutex_print);
  fprintf(fileout_basicmath_cubic,"\nsqrt(%lX) = %X\n", l, q.sqrt);
  pthread_mutex_unlock(&mutex_print);


*/
  pthread_mutex_lock(&mutex_print);
  fprintf(fileout_basicmath_cubic,"********* ANGLE CONVERSION ***********\n");
  pthread_mutex_unlock(&mutex_print);

  double res;
  
  for (X = 0.0; X <= 360.0; X += 1.0)
  {
       res = deg2rad(X);
       pthread_mutex_lock(&mutex_print);
       fprintf(fileout_basicmath_cubic,"%3.0f degrees = %.12f radians\n", X, res);
       pthread_mutex_unlock(&mutex_print);
  }
  
  for (X = 0.0; X <= (2 * PI + 1e-6); X += (PI / 180))
  {
       res = rad2deg(X);
       pthread_mutex_lock(&mutex_print);
       fprintf(fileout_basicmath_cubic,"%.12f radians = %3.0f degrees\n", X, res);
       pthread_mutex_unlock(&mutex_print);
  }
   
  
  return 0;
}
