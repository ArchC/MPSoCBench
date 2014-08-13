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




/********************************************************************************
* Original version
*	Stringsearch				                                
*	Parallel 1.0 Version															
* Authors:           	Yuchen Liang and Syed Muhammad Zeeshan Iqbal
* Supervisor:					Hakan Grahn	 						
* Section:          	Computer Science of BTH,Sweden
* GCC Version:				4.2.4								
* Environment: 				Kraken(Ubuntu4.2.4) with Most 8 Processors 				
* Start Date:       	15th September 2009					
* End Date:         	6th October 2009			
*		
*********************************************************************************/

/* +++Date last modified: 05-Jul-1997 */

/*
**        A Pratt-Boyer-Moore string search, written by Jerry Coffin
**  sometime or other in 1991.  Removed from original program, and
**  (incorrectly) rewritten for separate, generic use in early 1992.
**  Corrected with help from Thad Smith, late March and early
**  April 1992...hopefully it's correct this time. Revised by Bob Stout.
**
**  This is hereby placed in the Public Domain by its author.
**
**  10/21/93 rdg  Fixed bug found by Jeff Dunlop
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//#include <pthread.h>


#define MAX_WORKERS 64 
#define NO_OF_QUERY_STRINGS 1024

#define PRINT 			1 //  1 for print. 

#define PBMH	    	1 //	Praat-Boyer-Moore string search
#define BMH_CS      2 //	Boyer-Moore-Horspool Case sensitive search
#define BMH_CIS    	3 //	Boyer-Moore-Horspool Case In Sensitive search
#define BMH_CISA    4 //	Boyer-Moore-Horspool Case In Sensitive search with Accented Characters



typedef struct Params {
	int s_index;
	int e_index;
	int no;
	char *name;
} parameters;

static int partition_size;
static int search_method;

static parameters paramsArr[MAX_WORKERS];

char* search_strings;
char* find_strings[NO_OF_QUERY_STRINGS];
char* found_strings[NO_OF_QUERY_STRINGS];

char* findString;
char* inFileName;
char* patternFileName;
char* here; 

int fileSize;
int no_workers;


/******************************************************
acPthread
*******************************************************/
#include "../acPthread.h"

#define malloc acPthread_malloc
#define realloc acPthread_realloc
#define free acPthread_free
#define calloc acPthread_calloc

extern unsigned volatile int *lock;
extern pthread_mutex_t mutex_print;

extern int pthread_n_workers;
extern int pthread_free_for_start;

extern int pthread_finished;
extern int pthread_created;

//extern int flag;

unsigned volatile int procCounter;
volatile int barrier_in = 0;

FILE *fileout;

int m_argc;
char *m_argv[5];

int main(int argc, char *argv[])
{

	register int procNumber;
	
	AcquireGlobalLock();
	procNumber = procCounter++;
	ReleaseGlobalLock();

	if (procNumber == 0)
	{
		
		pthread_n_workers = NPROC;
                no_workers = pthread_n_workers;

		/* ARGUMENTS */
		m_argc = 5;
		m_argv[1] = "1";
		m_argv[2] = " "; //temp; 
		m_argv[3] = "SearchString01.txt";
		m_argv[4] = "testpattern5.txt";
	
		pthread_init();

		/* OUTPUT FILE */
		fileout = fopen("output","w");
		if (fileout == NULL){
			    printf("Error: fopen()\n");
			    exit(1);
		}
		
		
			
		pthread_mutex_init(&mutex_print, NULL);
		pthread_mutex_lock(&mutex_print);  
		printf("\n");
	        printf("\n");
        	printf("--------------------------------------------------------------------\n");
		printf("-------------------------  MPSoCBench  -----------------------------\n");
        	printf("-------------------- Running: stringsearch -------------------------\n");
        	printf("--------------- The results will be available in -------------------\n");
        	printf("--------------------- the output.dat file --------------------------\n");
        	printf("--------------------------------------------------------------------\n");
		printf("\n");
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
			if(pthread_created == 1)  pthread_executeThread();
		}
		

		
  	}
	_exit(0);  // To avoid cross-compiler exit routine
	return 0; // Never executed, just for compatibility
}

/******************************************************
acPthread
*******************************************************/



// pthread_attr_t string_attr;
pthread_attr_t string_attr;
pthread_mutex_t string_mutex;
pthread_t workers[MAX_WORKERS]; 


char *strsearch(const char *findString,const char *searchString) {
      register size_t shift;
      register size_t pos;
      			   size_t limit;
						   size_t len; 
						   size_t i;
						   size_t table[UCHAR_MAX + 1];
      char *findme;
      char *here;
               
      // Table Initialization
     len = strlen(findString);
      for (i = 0; i <= UCHAR_MAX; i++)                     
            table[i] = len;
      for (i = 0; i < len; i++)
            table[(unsigned char)findString[i]] = len - i - 1;
      findme = (char *)findString;
      
      // String Searching Starts here
      pos = len - 1;
			limit=strlen(searchString);
      while (pos < limit)
      {
            while( pos < limit &&
                  (shift = table[(unsigned char)searchString[pos]]) > 0)
            {
                  pos += shift;
            }
            if (0 == shift)
            {
                  if (0 == strncmp(findme,
                        here = (char *)&searchString[pos-len+1], len))
                  {
                        return(here);
                  }
                  else  pos++;
            }
      }
      return NULL;
}

/*
**  Input File Reading
*/
void readInputFile(){

	 FILE *pFile;
	 
	 fileSize=0;
	 
	 //printf("Opening Input File:<%s> \n",inFileName);
	 pFile= fopen(inFileName,"r");
	 
	 if (pFile== NULL){
		    printf("Error: fopen()");
		    exit(1);
	 }
	 
   fseek(pFile,0,SEEK_END);
   fileSize= ftell(pFile);
   rewind(pFile);
   
   search_strings = (char*) calloc(sizeof(char),fileSize+25);
   fread(search_strings,1,fileSize,pFile);
   
   if (ferror(pFile)){
   	   printf("Error: fread()\n");
   	   exit(1);
   }
  

   //fprintf(fileout,"Input File Size is:<%d>\n",fileSize);
   fclose(pFile);
   //printf("Done Reading Input File!\n");
     
}

void readPatternFile(){
	FILE *pFile;
	char line[100];
	int lcount=0;
	int index=1;

	 //printf("Opening Pattern File:<%s> \n",patternFileName);
	 pFile= fopen(patternFileName,"r");
	 
	 if (pFile== NULL){
		    printf("Error: fopen()\n");
		    exit(1);
	 }
	    
   
   while( index != EOF ) {
   	  	  index =fscanf( pFile,"%s", line);
   	      find_strings[lcount] = (char*) calloc(sizeof(char),strlen(line)+1);
   	      strcpy(find_strings[lcount],line);
   	      if (lcount > NO_OF_QUERY_STRINGS)
   	      		break;
   	      lcount++; 
   }//end-while
     
     	
   fclose(pFile);
   //printf("Done Reading Pattern File!\n");
}

void *searchStrings(void* param) {
	    
    
            parameters *tempParam= (parameters*)param;

	    int index;
	    size_t limit;
	    
	   
	    
	    pthread_mutex_lock(&mutex_print);
	    printf("The %s %d start searching.\n",tempParam->name,tempParam->no);
 	    pthread_mutex_unlock(&mutex_print);
	    
	    
	    for (index=tempParam->s_index; index<tempParam->e_index;index++){
	    	
		    	 if (search_method==PBMH ){
		    	 	  
		      		found_strings[index] = strsearch(find_strings[index],search_strings);	      		
		       }else if(search_method==BMH_CS){
		      	  limit=strlen(search_strings);
		      	  found_strings[index] = (char *)bmh_search((const char *)search_strings,(int)limit,(const char*) find_strings[index]);
		       }else if(search_method==BMH_CIS){
		      	  limit=strlen(search_strings);
		      	  found_strings[index] = (char *) bmhi_search(search_strings,(int)limit,find_strings[index]);
		       }else if(search_method==BMH_CISA){
		      		limit=strlen(search_strings);
		      	  found_strings[index] = (char *) bmha_search(search_strings,(int)limit,find_strings[index]);
		       }else {
		      	  printf("Undefined search method.\n");
		      	  exit(0);
		      }
	    	    	   
	    }
	    
	    
		
		 	    		
			pthread_exit(NULL);
		 	
		
	    
}

void init_workers(){
	
	pthread_attr_init(&string_attr);
	pthread_mutex_init(&string_mutex,NULL);
	pthread_attr_setdetachstate(&string_attr,PTHREAD_CREATE_JOINABLE);

	
}

void createWorkers(){
	long index;

	for ( index= 0 ; index<no_workers; index++){
	    pthread_create(&workers[index],&string_attr,(void *)searchStrings,&paramsArr[index]);
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
	   
          //printf("\nno_workers=%d\n", no_workers);
	  for (i=0;i<no_workers;i++) {
			   paramsArr[i].s_index=s_index;
			   paramsArr[i].e_index=s_index+partition_size;
			   paramsArr[i].name="Worker";
			   paramsArr[i].no=i+1;
			   //printf ("\ns_index->%d  e_index->%d   name-> %s   no->%d \n",paramsArr[i].s_index,paramsArr[i].e_index,paramsArr[i].name, paramsArr[i].no);
			   s_index+=partition_size;
			   }
 
	  // It handles even not of jobs and odd no of workers vice versa.
	   paramsArr[i-1].e_index=paramsArr[i-1].e_index+(NO_OF_QUERY_STRINGS%no_workers);
	   
	  
}

 void show_sub_tasks() {
 	pthread_mutex_lock(&mutex_print);
		int i;
			for (i=0;i<no_workers;i++) {
				   
				   printf("%d   ",paramsArr[i].s_index);
				   printf("%d   ",paramsArr[i].e_index);
				   printf("%s ",paramsArr[i].name);
				   printf("%d \n ",paramsArr[i].no);
		  }
 	pthread_mutex_unlock(&mutex_print);
}
 
void init(){
	  
	  
	  readInputFile();
	  readPatternFile();
	  
	  partition_size= NO_OF_QUERY_STRINGS/no_workers;
	  divide_into_sub_tasks();
	  show_sub_tasks();
	  
	  here=NULL;
	  
}   

void showFoundStrings(){
	int index;
	
 	pthread_mutex_lock(&mutex_print);
	for (index=0;index<NO_OF_QUERY_STRINGS;index++){
		here= found_strings[index];
		fprintf(fileout,"\"%s\" is%s in \"%s\"\n", find_strings[index], here ? "" : " not", inFileName);
          
	}
 	pthread_mutex_unlock(&mutex_print);
}

int main0(int argc,char *argv[])
{
	char *token;
	char *strSearchMthID; 
  
              
       if (argc<5 ||argc>5){
   	   printf("|-----------------------------------------------------------------------|\n");
       printf("	Error: Insufficient Parameters.                             \n");
       printf("	Maximum Workers are 8!\n");
       printf("	1 :  Pratt-Boyer-Moore         \n");
       printf("	2 :  Case-sensitive Boyer-Moore-Horspool \n");
       printf("	3 :  Case-Insensitive Boyer-Moore-Horspool \n");
       printf("	4 :  Boyer-Moore-Horspool (Case-insensitive with accented character translation)\n");
      	  
       printf("	Commands to run!                             \n");
       printf("	Command Format: OjbectFileName <StringSearchMethod> Workers <InputFileName> <PatternFileName>!\n");
       printf("	Example: parallel string search : ' ./parallel_stringsearch 1 2 InputFile.txt PatternFile.txt '!\n");
       printf("|---------------------------------------------------------------------- |\n");
       exit(0);
    }       
    
        //printf("\nargv[1] = %s argv[2] =%s argv[3]=%s argv[4]=%s\n",argv[1],argv[2],argv[3],argv[4]);
	strSearchMthID = argv[1];
        
	if (no_workers >MAX_WORKERS)
	{
		  printf("ERROR: Number of worker should be no more than %d\n", MAX_WORKERS);
		  exit(0);
	}   
	  
        inFileName=argv[3];     	          	  	
        patternFileName=argv[4];
     
	 
	init();
        init_workers();	  	
        
	 	 
	 if (strcmp(strSearchMthID,"1")==0){
			 search_method= PBMH;
	         	 pthread_mutex_lock(&mutex_print);
		 	 fprintf(fileout,"---***Pratt-Boyer-Moore string search***---\n");
		 	 printf("\n***Pratt-Boyer-Moore string search***\n");
	         	 pthread_mutex_unlock(&mutex_print);	
			 createWorkers();
			 waitForWorkersToFinish();
			 if (PRINT)
		  		 showFoundStrings();
   } else if (strcmp(strSearchMthID,"2")==0){  	
                         	 		 
			 search_method= BMH_CS;
	         	 pthread_mutex_lock(&mutex_print);
			 printf("\n***Case-sensitive Boyer-Moore-Horspool string search***\n");
		 	 fprintf(fileout,"---***Case-sensitive Boyer-Moore-Horspool string search***---\n");
	         	 pthread_mutex_unlock(&mutex_print);	
			 createWorkers();
			 waitForWorkersToFinish();
			 if (PRINT)
			 		 showFoundStrings();
  		 		 
   } else if (strcmp(strSearchMthID,"3")==0){ 		 
  		 search_method= BMH_CIS;
         	 pthread_mutex_lock(&mutex_print);
 	 		 fprintf(fileout,"***Case-Insensitive Boyer-Moore-Horspool string search*** \n");
			 printf("\n***Case-Insensitive Boyer-Moore-Horspool string search*** \n");
         	 pthread_mutex_unlock(&mutex_print);
	 		 createWorkers();
	 		 waitForWorkersToFinish();
  		 if (PRINT)
  		 		 showFoundStrings();
   }else if (strcmp(strSearchMthID,"4")==0){
	  	 search_method= BMH_CISA;
         	 pthread_mutex_lock(&mutex_print);		
                  fprintf(fileout,"***Boyer-Moore-Horspool (Case-insensitive with accented character translation) string search***\n");
                  printf("\n***Boyer-Moore-Horspool (Case-insensitive with accented character translation) string search***\n");
		pthread_mutex_unlock(&mutex_print);		
	 		 createWorkers();
	 		 waitForWorkersToFinish();
  		 if (PRINT)
		 
  		 		 showFoundStrings();
   }else {
       printf("Invalid Method ID.\n");	
  }	

    
   return 0;
}




