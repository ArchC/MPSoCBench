#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define UNLIMIT

#define MAXARRAY 100000 /* this number, if too large, will cause a seg. fault!! */

struct myStringStruct {
  char qstring[128];
};

#ifndef acPthread_H_
#include "../../acPthread.h"
#endif
extern pthread_mutex_t mutex_print;
extern FILE* fileout_qsort;
extern FILE* filein_qsort;

int compare(const void *elem1, const void *elem2)
{
  int result;
  
  result = strcmp((*((struct myStringStruct *)elem1)).qstring, (*((struct myStringStruct *)elem2)).qstring);

  return (result < 0) ? 1 : ((result == 0) ? 0 : -1);
}


int
main_qsort(int argc, char *argv[])
{
  struct myStringStruct array[MAXARRAY];
  
 
  int i,count=0;
 
    while((fscanf(filein_qsort, "%s", &array[count].qstring) == 1) && (count < MAXARRAY)) {
	 count++;
    }
  

  qsort(array,count,sizeof(struct myStringStruct),compare);

  pthread_mutex_lock(&mutex_print); 
  fprintf(fileout_qsort,"\nSorting %d elements.\n\n",count);
  for(i=0;i<count;i++)
    fprintf(fileout_qsort,"%s\n", array[i].qstring);
  pthread_mutex_unlock(&mutex_print); 

  return 0;
}
