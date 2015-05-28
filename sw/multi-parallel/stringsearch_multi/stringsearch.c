
/********************************************************************************
*						      			       				   
* 																Stringsearch				                                
*															Parallel 1.0 Version															
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


#define MAX_WORKERS_LOCAL 16 //8
#define NO_OF_QUERY_STRINGS 1024

#define PRINT 	1 //  1 for print. 0 for no print

#define PBMH	    	1 //	Praat-Boyer-Moore string search
#define BMH_CS      2 //	Boyer-Moore-Horspool Case sensitive search
#define BMH_CIS    	3 //	Boyer-Moore-Horspool Case In Sensitive search
#define BMH_CISA    4 //	Boyer-Moore-Horspool Case In Sensitive search with Accented Characters


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

extern FILE *fileout_stringsearch;
extern FILE *filein1_stringsearch;
extern FILE *filein2_stringsearch;

int main_stringsearch ()
{
	/* NUMBER OF WORKER THREADS */
	
	 
	int m_argc;
	char *m_argv[5];

	// pthread_n_workers = atoi(temp);
	
	/* ARGUMENTS */
	m_argc = 5;
	m_argv[1] = "1";
	m_argv[2] = ""; 
	m_argv[3] = ""; // input file
	m_argv[4] = ""; // output file
	

	

		
	main0_stringsearch(m_argc,m_argv);

	fclose(fileout_stringsearch);
	fclose(filein1_stringsearch);
	fclose(filein2_stringsearch);
  	
	return 0; // Never executed, just for compatibility
}
/******************************************************
acPthread
*******************************************************/


typedef struct Params {
	int s_index;
	int e_index;
	int no;
	char *name;
} parameters;

static int partition_size;
static int search_method;

static parameters paramsArr[MAX_WORKERS_LOCAL];

char* search_strings;
char* find_strings[NO_OF_QUERY_STRINGS];
char* found_strings[NO_OF_QUERY_STRINGS];

char* findString;
char* inFileName;
char* patternFileName;
char* here; 

int fileSize;
int no_workers;

pthread_attr_t string_attr;
pthread_mutex_t string_mutex;
pthread_t workers[MAX_WORKERS_LOCAL]; 


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

void readInputFile()
{
	 
	

   	fileSize=0;
 
	fseek(filein1_stringsearch,0,SEEK_END);
	fileSize= ftell(filein1_stringsearch);
   
	

	//rewind(filein1_stringsearch);
	fseek(filein1_stringsearch,0,SEEK_END);
	
	

   search_strings = (char*) calloc(sizeof(char),fileSize+25);
   fread(search_strings,1,fileSize,filein1_stringsearch);

   

   
   if (ferror(filein1_stringsearch)){
   	   printf("Error: fread()\n");
   	   exit(1);
   }


	
}

void readPatternFile(){

	char line[100];
	int lcount=0;
	int index=1;

	while( index != EOF ) {
   	      index =fscanf( filein2_stringsearch ,"%s", line);
   	      find_strings[lcount] = (char*) calloc(sizeof(char),strlen(line)+1);
   	      strcpy(find_strings[lcount],line);
   	      if (lcount > NO_OF_QUERY_STRINGS)
   	      		break;
   	      lcount++; 
   }
     
}

void *searchStrings(void* param) {
	    
	    parameters *tempParam= (parameters*)param;

	    int index;
	    size_t limit;
	    
	    /*if (PRINT)
		{
		 	pthread_mutex_lock(&mutex_print);
	  		printf("Stringsearch: The %s %d started.\n",tempParam->name,tempParam->no);
		 	pthread_mutex_unlock(&mutex_print);
		}*/
	    
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
	    
	    /*if (PRINT)
		{
		 	pthread_mutex_lock(&mutex_print);	    	
			printf("Stringsearch: The %s %d finished.\n",tempParam->name,tempParam->no);
			pthread_mutex_unlock(&mutex_print);	    		
			
		 	
		}*/

	    pthread_exit(NULL);
	    
}

void init_workers_stringsearch(){
	
	pthread_attr_init(&string_attr);
	pthread_mutex_init(&string_mutex,NULL);
	//pthread_attr_setdetachstate(&string_attr,3);  // 3 indicates that software is stringsearch
}

void createWorkers_stringsearch(){
	long index;
	for ( index= 0 ; index<no_workers; index++){
                    
		    pthread_create(&workers[index],&string_attr,(void *)searchStrings,&paramsArr[index]);
	}
	
	pthread_executeThread();
	
}
void waitForWorkersToFinish_stringsearch(){
	int index;
	    for ( index= 0 ; index<no_workers; index++)
            {
	    	   pthread_join(workers[index],NULL);
		   //join_stringsearch(workers[index],NULL);
	    }	   
 }                                               
 
void divide_into_sub_tasks_stringsearch(){

	  int i;
	  int s_index=0;
	   
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

 void show_sub_tasks_stringsearch() {
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
 
void init_stringsearch(){
	  

	  
	  readInputFile();

	
	  readPatternFile();

	
	  
	  partition_size= NO_OF_QUERY_STRINGS/no_workers;
	  divide_into_sub_tasks_stringsearch();
	  //show_sub_tasks_stringsearch();
	  
	  here=NULL;
	  
}   

void showFoundStrings(){
	int index;
	
	pthread_mutex_lock(&mutex_print);
	
	for (index=0;index<NO_OF_QUERY_STRINGS;index++){
		here= found_strings[index];
		fprintf(fileout_stringsearch,"\"%s\" is%s in the input file\n", find_strings[index], here ? "" : " not");
	}
 	pthread_mutex_unlock(&mutex_print);
}

int main0_stringsearch(int argc,char *argv[])
{
	char *token;
	char *strSearchMthID; 
  
              
       if (argc<5 ||argc>5){
   	   printf("|-----------------------------------------------------------------------|\n");
       printf("	Error: Insufficient Parameters.                             \n");
       printf("	Maximum Workers are 16!\n"); // originaly, maximum workers was 8
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
    
	
	strSearchMthID = argv[1];

	//no_workers= atoi(argv[2]);
	//no_workers= atoi(argv[2]); //pthread_n_workers;

	no_workers = pthread_threads_per_software;
	


	if (no_workers >
MAX_WORKERS_LOCAL)
	{
		  printf("ERROR: Number of worker should be no more than 8\n");
		  exit(0);
	}   
	  
        inFileName=argv[3];     	          	  	
        patternFileName=argv[4];

	
    
	init_stringsearch();
	
	 init_workers_stringsearch();	  	
	 	 
	 if (strcmp(strSearchMthID,"1")==0){
			 search_method= PBMH;
	         	 pthread_mutex_lock(&mutex_print);
		 	 fprintf(fileout_stringsearch,"---***Pratt-Boyer-Moore string search***---\n");
	         	 pthread_mutex_unlock(&mutex_print);	


	

			 createWorkers_stringsearch();
			 waitForWorkersToFinish_stringsearch();
			 if (PRINT)
		  		 showFoundStrings();
   } else if (strcmp(strSearchMthID,"2")==0){  		 		 
			 search_method= BMH_CS;
	         	 pthread_mutex_lock(&mutex_print);
		 	 fprintf(fileout_stringsearch,"---***Case-sensitive Boyer-Moore-Horspool string search***---\n");
	         	 pthread_mutex_unlock(&mutex_print);	
			 createWorkers_stringsearch();
			 waitForWorkersToFinish_stringsearch();
			 if (PRINT)
			 		 showFoundStrings();
  		 		 
   } else if (strcmp(strSearchMthID,"3")==0){ 		 
  		 search_method= BMH_CIS;
         	 pthread_mutex_lock(&mutex_print);
 	 		 fprintf(fileout_stringsearch,"---***Case-Insensitive Boyer-Moore-Horspool string search***--- \n");
         	 pthread_mutex_unlock(&mutex_print);
	 		 createWorkers_stringsearch();
	 		 waitForWorkersToFinish_stringsearch();
  		 if (PRINT)
  		 		 showFoundStrings();
   }else if (strcmp(strSearchMthID,"4")==0){
	  	 search_method= BMH_CISA;
         	 pthread_mutex_lock(&mutex_print);		
                  fprintf(fileout_stringsearch,"---***Boyer-Moore-Horspool (Case-insensitive with accented character translation) string search***---\n");
		pthread_mutex_unlock(&mutex_print);		
	 		 createWorkers_stringsearch();
	 		 waitForWorkersToFinish_stringsearch();
  		 if (PRINT)
		 
  			 showFoundStrings();
   }else {
       printf("Invalid Method ID.\n");	
  }	
   
   
   return 0;
}




