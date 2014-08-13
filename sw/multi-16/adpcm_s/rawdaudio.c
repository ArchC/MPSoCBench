/* testd - Test adpcm decoder */

#include "adpcm.h"
#include <stdio.h>

struct adpcm_state state_decoder;

#define NSAMPLES 1000

#ifndef acPthread_H_
#include "../../acPthread.h"
#endif

extern FILE* filein_adpcmdecoder;
extern FILE* fileout_adpcmdecoder;

extern pthread_mutex_t mutex_print;

char	dec_abuf[NSAMPLES/2];
short	dec_sbuf[NSAMPLES];

void main_adpcmdecoder() {
    int n;

    while ( (n=fread(dec_sbuf, 1, NSAMPLES/2,filein_adpcmdecoder)) > 0 )
    {
	adpcm_decoder(dec_abuf, dec_sbuf, n*2, &state_decoder);
	fwrite(dec_sbuf, 1, n*4, fileout_adpcmdecoder);
	
    }

    //fprintf(stderr, "Final valprev=%d, index=%d\n",state.valprev, state.index);
    
    return;
}
