/* testc - Test adpcm coder */

#include "adpcm.h"
#include <stdio.h>

struct adpcm_state state_encoder;

#define NSAMPLES 1000

#ifndef acPthread_H_
#include "../../acPthread.h"
#endif

extern FILE* filein_adpcmencoder;
extern FILE* fileout_adpcmencoder;

extern pthread_mutex_t mutex_print;

char	cod_abuf[NSAMPLES/2];
short	cod_sbuf[NSAMPLES];

void main_adpcmencoder () {
    int n;
    int count = 0;
	
    
    while ( (n=fread(cod_sbuf, 1, NSAMPLES*2,filein_adpcmencoder)) > 0 )
    {
	
	adpcm_coder(cod_sbuf, cod_abuf, n/2, &state_encoder);
	fwrite(cod_abuf, 1, n/4, fileout_adpcmencoder);
	
    }


   // fprintf(stderr, "Final valprev=%d, index=%d\n",state.valprev, state.index);

    return;
}



