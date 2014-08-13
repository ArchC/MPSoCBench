/* NIST Secure Hash Algorithm */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "sha.h"


#ifndef acPthread_H_
#include "../../acPthread.h"
#endif

extern pthread_mutex_t mutex_print;
extern FILE *filein_sha;

int main_sha ()
{
    // FILE *fin;
    SHA_INFO sha_info;
	
	if (filein_sha == NULL) {
	    printf("error opening filein_sha for reading (sha)\n");
        }
        else
        {
	  sha_stream(&sha_info, filein_sha);

	  pthread_mutex_lock(&mutex_print);
	  sha_print(&sha_info);
	  pthread_mutex_unlock(&mutex_print);	

	  fclose(filein_sha);
	}
    
    
    return(0);
}
