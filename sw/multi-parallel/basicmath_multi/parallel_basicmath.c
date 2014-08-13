/********************************************************************************
*						      			       				   
* Basicmath				                                
* Parallel 1.0 Version with Pthread 
* Authors:           	Yuchen Liang and Syed Muhammad Zeeshan Iqbal
* Supervisor:					Hakan Grahn	 						
* Section:          	Computer Science of BTH,Sweden
* GCC Version:				4.2.4								
* Environment: 				Kraken(Ubuntu4.2.4) with Most 8 Processors 				
* Start Date:       	15th September 2009					
* End Date:         	6th October 2009			
*		
*********************************************************************************/

#include "snipmath.h"
#include <math.h>
#include <stdio.h>
//#include <pthread.h>
#define  PRINT  1 // Print Switch

#define MAX_WORKERS 16

#define 		CUBIC 		1
#define 		ISQRT			2
#define 		ULSQRT		3
#define 		DEGTORAD	4
#define 		RADTODEG	5

// Switch to do large or small calculations.
#define     LARGE_DATA_SET  1 
#define     SMALL_DATA_SET  2 


#define     ONE_KILO_NUMBERS 1024
#define     ONE_MEGA_NUMBERS 1024*1024
#define     ONE_GIGA_NUMBERS 1024*1024*1024



/******************************************************
acPthread
*******************************************************/
#ifndef acPthread_H_
#include "../acPthread_p.h"
#endif

#define malloc acPthread_malloc
#define realloc acPthread_realloc
#define free acPthread_free
#define calloc acPthread_calloc

extern pthread_mutex_t mutex_print;
extern int pthread_threads_per_software; // number of threads per software 

extern FILE *fileout_basicmath;
/******************************************************
acPthread
*******************************************************/




pthread_attr_t basicmath_attr;
pthread_t workers[MAX_WORKERS]; 
int math_operation;
int data_set_type;
int no_workers;


typedef struct Ilimits{ // Limits of ineger type
		int _start;
		int _end;
		int _offset;
}ilimit;

typedef struct ULlimits{ // Limits of unsigned long type
	  unsigned long _start;
	  unsigned long _end;
	  unsigned long _offset;
}ullimit;

typedef struct DoubleLimits{  // Limits of double type
	   double _start;
	   double _end;
	   double _offset;
}dlimit;


  static ilimit  _ilim,_ilimParams[MAX_WORKERS];
  static ullimit _ullim,_ullimParams[MAX_WORKERS];
  static dlimit  _dlim1,_dlim2,_dlimParams1[MAX_WORKERS],_dlimParams2[MAX_WORKERS];
  static dlimit  _adlim,_bdlim,_cdlim,_ddlim,_adlimParams[MAX_WORKERS];  // double type limits for variable's a,b,c,d


int main_basicmath()
{
	/* NUMBER OF WORKER THREADS */
	
	// char *temp = argv[1];

	int m_argc;
	char *m_argv[4];

	no_workers= pthread_threads_per_software;

	/* ARGUMENTS */
	m_argc = 4; // ./parallel_basicmath x y z"
	m_argv[1] = "2";  // mathematical operation-> can assume values 1,2,3,4, or 5
	m_argv[2] = "";  // number of workers (no_workers)
        m_argv[3] = "2";  // data set -> can assume values 1 (large) or 2 (small)
    
	

	main0_basicmath(m_argc,m_argv);
	fclose(fileout_basicmath);
	return 0;
}
/******************************************************
acPthread
*******************************************************/



void intializeLimits(){
		
	
	if (data_set_type==LARGE_DATA_SET){
  		//Initialization of integer type limits
	  	_ilim._start=0;
	  	_ilim._end=  1*ONE_GIGA_NUMBERS;
	  	_ilim._offset=1;
	  	
	  	//Initialization of unsigned long type limits
	    _ullim._start=1072497001;
	  	_ullim._end=1072497001+ (1*ONE_MEGA_NUMBERS);
	  	_ullim._offset=1;
	  	
	  	//Initialization of double type limits
	  	
	  	
	  	_adlim._start=1.0,_adlim._end=16.0,_adlim._offset=1e-10;
	  	_bdlim._start=8.0,_bdlim._end=10.0,_bdlim._offset=0.25;
	  	_cdlim._start=5.0,_cdlim._end=15.0,_cdlim._offset=0.61;
	  	_ddlim._start=-1.0,_ddlim._end=-5.0,_ddlim._offset=0.451;
	  	
	  	
	  	
	  	_dlim1._start=0.0;
	  	_dlim1._end=360.0;
	  	_dlim1._offset=1e-9;
	  	
	  	_dlim2._start=0.0;
	  	_dlim2._end=(6 * PI + 1e-6);
	  	_dlim2._offset=(PI / 1e11);
	  	
   }else if (data_set_type==SMALL_DATA_SET){
  	   //Initialization of integer type limits
	  	_ilim._start=0;
	  	_ilim._end=ONE_KILO_NUMBERS;
	  	_ilim._offset=1;
	  	
	  	//Initialization of unsigned long type limits
	  	_ullim._start=1072497001;
	  	_ullim._end=  1072497001 +(10*ONE_KILO_NUMBERS);
	  	_ullim._offset=1;
	  	
	  	//Initialization of double type limits
	  	_adlim._start=1.0,_adlim._end=2.0,_adlim._offset=0.5;
	  	_bdlim._start=10.0,_bdlim._end=9.0,_bdlim._offset=0.25;
	  	_cdlim._start=5.0,_cdlim._end=7.0,_cdlim._offset=0.61;
	  	_ddlim._start=-1.0,_ddlim._end=-5.0,_ddlim._offset=0.451;
	  	
  	
	  	_dlim1._start=0.0;
	  	_dlim1._end=360.0;
	  	_dlim1._offset=1;
	  	
	  	_dlim2._start=0.0;
	  	_dlim2._end=(6 * PI + 1e-6);
	  	_dlim2._offset=(PI /1e2);
  	
  }
}//end-initializeLimits


void solveCubicEquations(int workerID){
	
	double  a1,b1,c1,d1;
  double  x[3];
  int     solutions, index;

	//pthread_mutex_lock(&mutex_print);
	//fprintf(fileout_basicmath,"Basicmath: Worker %d started (cubic equations).\n",workerID);
	//printf("Basicmath: Worker %d started (cubic equations)\n",workerID);
	//pthread_mutex_unlock(&mutex_print);

        
       for(a1=_adlimParams[workerID]._start;a1<_adlimParams[workerID]._end;a1+=_adlim._offset)
       {
	  
    	   for(b1=_bdlim._start;b1>_bdlim._end;b1-=_bdlim._offset)
           {
     		for(c1=_cdlim._start;c1<_cdlim._end;c1+=_cdlim._offset)
                {
		      for(d1=-_ddlim._start;d1>_ddlim._end;d1-=_ddlim._offset)
                      {
				SolveCubic(a1, b1, c1, d1, &solutions, x);  
				//printf("\nantes de entrar no mutex pritn\n");
				if (PRINT){
					 
					 pthread_mutex_lock(&mutex_print);
					 fprintf(fileout_basicmath,"Solutions:");
					 for(index=0;index<solutions;index++)
					 	fprintf(fileout_basicmath," %f",x[index]);
					 fprintf(fileout_basicmath,"\n");
					 pthread_mutex_unlock(&mutex_print);
				}//endif
				
	
				
		      }// end-for

	
		 }//end-for
	  

	  }//end-for
	  
   
      }//end-for
	
}

void calcISqrt(int workerID){
	int index;
	struct int_sqrt q;
		
	pthread_mutex_lock(&mutex_print);
	fprintf(fileout_basicmath,"Basicmath: Worker %d started (integer sqr root).\n",workerID);
	//printf("Basicmath: Worker %d started (integer sqr root).",workerID);
	pthread_mutex_unlock(&mutex_print);

  for (index = _ilimParams[workerID]._start; index < _ilimParams[workerID]._end; index+=_ilim._offset)
    {
      usqrt(index, &q);
			// remainder differs on some machines
			if (PRINT)
			{
				pthread_mutex_lock(&mutex_print);
     				fprintf(fileout_basicmath,"sqrt(%3d) = %2d\n",index, q.sqrt);
				pthread_mutex_unlock(&mutex_print);
			}
    }//end-for
	
}

void calcULSqrt(int workerID){
	 unsigned long l = 0x3fed0169L;
	 struct int_sqrt q;
	 
	
	/*pthread_mutex_lock(&mutex_print);
	fprintf(fileout_basicmath,"\nBasicmath: Worker %d started (long sqr roots).\n",workerID);
	printf("\nBasicmath: Worker %d started (long sqr root)\n",workerID);
	pthread_mutex_unlock(&mutex_print);*/

  for (l =_ullimParams[workerID]._start ; l <_ullimParams[workerID]._end; l+=_ullim._offset)
    {  
    	 usqrt(l, &q);
    	 if (PRINT)
 	 {
	       pthread_mutex_lock(&mutex_print);
	       fprintf(fileout_basicmath,"sqrt(%lX) = %X\n", l, q.sqrt);
	       pthread_mutex_unlock(&mutex_print);
	}
    }
	
}

void degToRadianConv(int workerID){
	double X;
				
	//pthread_mutex_lock(&mutex_print);
	//fprintf(fileout_basicmath,"Basicmath: Worker %d started (degree to radian angle conversion).\n",workerID);
	//printf("Basicmath: Worker %d started (degree to radian angle conversion).\n",workerID);
	//pthread_mutex_unlock(&mutex_print);


   
  for (X =_dlimParams1[workerID]._start; X <=_dlimParams1[workerID]._end; X +=_dlim1._offset){
    	 if (PRINT){
		pthread_mutex_lock(&mutex_print);
    		fprintf(fileout_basicmath,"%3.0f degrees = %.12f radians\n", X, deg2rad(X));
		pthread_mutex_unlock(&mutex_print);
	}
    	 else
    	   		 deg2rad(X);
    	}

}

void radianToDegConv(int workerID){
	double X;


	//pthread_mutex_lock(&mutex_print);
	//fprintf(fileout_basicmath,"Basicmath: Worker %d started (radian to degree angle conversion).\n",workerID);
	//printf("Basicmath: Worker %d started (radian to degree angle conversion).\n",workerID);
	//pthread_mutex_unlock(&mutex_print);


	for (X =_dlimParams2[workerID]._start; X <=_dlimParams2[workerID]._end; X +=_dlim2._offset){
  	   if (PRINT)
    	   {
		pthread_mutex_lock(&mutex_print);
		fprintf(fileout_basicmath,"%.12f radians = %3.0f degrees\n", X, rad2deg(X));
		pthread_mutex_unlock(&mutex_print);
	    }
  		 else
  				rad2deg(X);	 
  	}
	
}

void init_workers_basicmath(){
	
	pthread_attr_init(&basicmath_attr);
	//pthread_attr_setdetachstate(&basicmath_attr,1); // 1 indicates that software is basicmath
	
}

void *doWork(void *workerID){
	long id = (long)workerID;
	
	//Each thread performs following five tasks according to assigned work.
	 if (math_operation ==CUBIC )
	 		 solveCubicEquations(id);
	 else if (math_operation ==ISQRT)
	 		 calcISqrt(id);
	 else if (math_operation ==ULSQRT)
			 calcULSqrt(id);
	 else if (math_operation ==DEGTORAD)
		   degToRadianConv(id);
	 else if (math_operation ==RADTODEG)
		   radianToDegConv(id);
	 else
	    printf("Undefined math operation\n");	
	 		
	 /*if (PRINT)
	 {	
		  pthread_mutex_lock(&mutex_print);	
		  printf("Basicmath: Worker %d finished.\n",id);
		  fprintf(fileout_basicmath,"Worker %d has completed work.\n",id);
		  pthread_mutex_unlock(&mutex_print);	
	 }*/
}

void createWorkers_basicmath(){
	long index;
		
	for ( index= 0 ; index<no_workers; index++){
		pthread_create(&workers[index],&basicmath_attr,(void *)doWork,(void *)index);
	}
	pthread_executeThread();
}
void waitForWorkersToFinish_basicmath(){
	int index;
	    for ( index= 0 ; index<no_workers; index++)
	     {
	    	   pthread_join(workers[index],NULL);
		  
	     }
	    	   
 }                                               
 
void divide_into_sub_tasks_basicmath(){

	  int i,temp;
	  int isindex;
	  unsigned long ulsindex,ulTemp;
	  double dsindex1,dsindex2,adsindex;
	  
	  adsindex= _adlim._start;
	  isindex=		_ilim._start;
	  ulsindex= _ullim._start;
	  
	  dsindex1= _dlim1._start;
	  dsindex2= _dlim2._start;
	  
	  // Total long numbers
	  ulTemp=_ullim._end-_ullim._start;
	  
	  for (i=0;i<no_workers;i++) {
	  	
	  	    
	  	  	
	  	 	 _adlimParams[i]._start=adsindex;
	  	   _adlimParams[i]._end = _adlimParams[i]._start+(_adlim._end/no_workers);
			    adsindex+=(_adlim._end/no_workers);
	  	
	  	   _ilimParams[i]._start=isindex;
	  	   _ilimParams[i]._end = _ilimParams[i]._start+(_ilim._end/no_workers);
			    isindex+=(_ilim._end/no_workers);
			   
			   _ullimParams[i]._start=ulsindex;
	  	   _ullimParams[i]._end = _ullimParams[i]._start+(ulTemp/no_workers);
			    ulsindex+=(ulTemp/no_workers);
			   
			   _dlimParams1[i]._start=dsindex1;
	  	   _dlimParams1[i]._end = _dlimParams1[i]._start+(_dlim1._end/no_workers);
			    dsindex1+=(_dlim1._end/no_workers);
			   
			   temp=_dlim2._end;
			   _dlimParams2[i]._start=dsindex2;
	  	   _dlimParams2[i]._end = _dlimParams2[i]._start+(temp/no_workers);
			    dsindex2+=(temp/no_workers);
			    
		   }
 
	  // It handles even and odd number total work.
	   
	   _ilimParams[i-1]._end =_ilimParams[i-1] . _end+(_ilim._end%no_workers);
	   _ullimParams[i-1]._end=_ullimParams[i-1]._end+(_ullim._end%no_workers);
	   
	   
	  
}

  void show_sub_tasks_basicmath() {

		int i;
		
		  pthread_mutex_lock(&mutex_print);
	  	  fprintf(fileout_basicmath,"Tasks For Cubic Equations!\n");
			for (i=0;i<no_workers;i++) {
				                 
				   fprintf(fileout_basicmath,"%3.2f   ",_adlimParams[i]._start);
				   fprintf(fileout_basicmath,"%3.2f  \n",_adlimParams[i]._end);
				   
		  }
		  
		  fprintf(fileout_basicmath,"Tasks for Integer Type Square Roots!\n");
		  for (i=0;i<no_workers;i++) {
				   
				   fprintf(fileout_basicmath,"%d   ",_ilimParams[i]._start);
				   fprintf(fileout_basicmath,"%d   \n",_ilimParams[i]._end);
				   
		  }
		  
		  printf("Tasks for Long Type Square Roots!\n");
		  for (i=0;i<no_workers;i++) {
				   
				   fprintf(fileout_basicmath,"%d   ",_ullimParams[i]._start);
				   fprintf(fileout_basicmath,"%d   \n",_ullimParams[i]._end);
				   
		  }
		  
		  fprintf(fileout_basicmath,"Tasks For Degree to Radian Conversion\n");
		  for (i=0;i<no_workers;i++) {
				   
				   fprintf(fileout_basicmath,"%3.2f   ",_dlimParams1[i]._start);
				   fprintf(fileout_basicmath,"%3.2f   \n",_dlimParams1[i]._end);
				   
		  }
		  
		  fprintf(fileout_basicmath,"Tasks For Radian to Degree Conversion\n");
		   for (i=0;i<no_workers;i++) {
				   
				   fprintf(fileout_basicmath,"%3.2f   ",_dlimParams2[i]._start);
				   fprintf(fileout_basicmath,"%3.2f   \n",_dlimParams2[i]._end);
				   
		  }
  		  pthread_mutex_unlock(&mutex_print);
		  
}


/* The printf's may be removed to isolate just the math calculations */

int main0_basicmath(int argc,char *argv[])
{
	 char *token;
	 char *strMathOpID;
	 char *dataSetType;
		

	 
	 if (argc<4 ||argc>4){
   	   printf("|-----------------------------------------------------------------------|\n");
       printf("	Error: Insufficient Parameters.                             \n");
       printf("	Maximum Workers are 8!\n");             
       printf("	AVAILABLE MATHEMATICAL OPERATIONS         \n");
       printf("	1 :  SOLVE CUBIC EQUATIONS         \n");
       printf("	2 :  CALCULATE INTEGER SQR ROOTS \n");
       printf("	3 :  CALCULATE LONG SQR ROOTS \n");
       printf("	4 :  PERFORM DEGREE TO RADIAN ANGLE CONVERSION\n");
       printf("	5 :  PERFORM RADIAN TO DEGREE ANGLE CONVERSION\n");
       printf("	AVAILABLE DATASETS         \n");
       printf("	1 :  LARGE DATA SET    \n");
       printf("	2 :  SMALL DATA SET    \n");
       
      	  
       printf("	Commands to run!                             \n");
       printf("	Command Format: OjbectFileName <Math Operation> Workers DataSetSize !\n");
       printf("	Example: parallel basic math : ' ./parallel_basicmath 1 2 1'!\n");
       printf("|---------------------------------------------------------------------- |\n");
       exit(0);
    
      }

       strMathOpID = argv[1];
         
       // no_workers= atoi(argv[2]);  //pthread_n_workers;
 	//printf("\nn_workers inicializado com %d em main0_basicmath\n",no_workers);   
       if (no_workers >MAX_WORKERS)
       {
	         printf("ERROR: Number of worker should be no more than 8\n");
	         exit(0);
	}   
    
   	     
   	   
   	   dataSetType =argv[3];
       
        
	 

		  		 
	 if (strcmp(strMathOpID,"1")==0)
	  		math_operation=CUBIC;
	 else if (strcmp(strMathOpID,"2")==0)
	 			math_operation=ISQRT;
	 else if (strcmp(strMathOpID,"3")==0)
	 			math_operation=ULSQRT;
	 else if (strcmp(strMathOpID,"4")==0)
	 			math_operation=DEGTORAD ;
	 else if (strcmp(strMathOpID,"5")==0)
	 			math_operation=RADTODEG;
	 else{
	 	  printf("Undefined math operation!\n");
	 	  exit (0);
	 	}
	 
	 
	 if (strcmp(dataSetType,"1")==0)
	 			data_set_type=LARGE_DATA_SET ;
	 else if (strcmp(dataSetType,"2")==0)
	 			data_set_type=SMALL_DATA_SET;
	 else{
	 	  printf("Undefined data set type!\n");
	 	  exit (0);
	 	}
		 
        intializeLimits();
	divide_into_sub_tasks_basicmath();
		 
	init_workers_basicmath();

	

	createWorkers_basicmath();
	waitForWorkersToFinish_basicmath();
	
	//puts("Finish Working");
	return 0;
}
