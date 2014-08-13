#line 156 "c.m4.null"

#line 1 "mdmain.C"
/*************************************************************************/
/*                                                                       */
/*  Copyright (c) 1994 Stanford University                               */
/*                                                                       */
/*  All rights reserved.                                                 */
/*                                                                       */
/*  Permission is given to use, copy, and modify this software for any   */
/*  non-commercial purpose as long as this copyright notice is not       */
/*  removed.  All other uses, including redistribution in whole or in    */
/*  part, are forbidden without prior written permission.                */
/*                                                                       */
/*  This software is provided with absolutely no warranty and no         */
/*  support.                                                             */
/*                                                                       */
/*************************************************************************/

 
#line 17
	#include <sys/time.h>
#line 17
	#include <unistd.h>
#line 17
	#include <stdlib.h>
#line 17
	#include <malloc.h>	
#line 17

#include "stdio.h"
#include "parameters.h"
#include "mdvar.h"
#include "water.h"
#include "wwpot.h"
#include "cnst.h"
#include "mddata.h"
#include "fileio.h"
#include "split.h"
#include "global.h"

/************************************************************************/

/* routine that implements the time-steps. Called by main routine and calls others */
double MDMAIN(long NSTEP, long NPRINT, long NSAVE, long NORD1, long ProcID)
{
    double XTT;
    long i;
    double POTA,POTR,POTRF;
    double XVIR,AVGT,TEN;
    double TTMV = 0.0, TKIN = 0.0, TVIR = 0.0;

    /*.......ESTIMATE ACCELERATION FROM F/M */
    INTRAF(&gl->VIR,ProcID);
  


    {	
#line 45
	while (barrier_wait(&(gl->start)) == 1);
#line 45
};

   

    INTERF(ACC,&gl->VIR,ProcID);

    
    {	
#line 52
	while (barrier_wait(&(gl->start)) == 1);
#line 52
};


   

    /* MOLECULAR DYNAMICS LOOP OVER ALL TIME-STEPS */

    for (i=1;i <= NSTEP; i++) {
        TTMV=TTMV+1.00;

        /* reset simulator stats at beginning of second time-step */

        /* POSSIBLE ENHANCEMENT:  Here's where one start measurements to avoid
           cold-start effects.  Recommended to do this at the beginning of the
           second timestep; i.e. if (i == 2).
           */

        /* initialize various shared sums */
        if (ProcID == 0) {
            long dir;
            if (i >= 2) {
                {	
#line 73
	long time(); (gl->trackstart) = time(0);
#line 73
};
            }
            gl->VIR = 0.0;
            gl->POTA = 0.0;
            gl->POTR = 0.0;
            gl->POTRF = 0.0;
            for (dir = XDIR; dir <= ZDIR; dir++)
                gl->SUM[dir] = 0.0;
        }

        if ((ProcID == 0) && (i >= 2)) {
            {	
#line 84
	long time(); (gl->intrastart) = time(0);
#line 84
};
        }

        {	
#line 87
	while (barrier_wait(&(gl->start)) == 1);
#line 87
};
        PREDIC(TLC,NORD1,ProcID);
        INTRAF(&gl->VIR,ProcID);
        {	
#line 90
	while (barrier_wait(&(gl->start)) == 1);
#line 90
};

        if ((ProcID == 0) && (i >= 2)) {
            {	
#line 93
	long time(); (gl->intraend) = time(0);
#line 93
};
            gl->intratime += gl->intraend - gl->intrastart;
        }


        if ((ProcID == 0) && (i >= 2)) {
            {	
#line 99
	long time(); (gl->interstart) = time(0);
#line 99
};
        }

        INTERF(FORCES,&gl->VIR,ProcID);

        if ((ProcID == 0) && (i >= 2)) {
            {	
#line 105
	long time(); (gl->interend) = time(0);
#line 105
};
            gl->intertime += gl->interend - gl->interstart;
        }

        if ((ProcID == 0) && (i >= 2)) {
            {	
#line 110
	long time(); (gl->intrastart) = time(0);
#line 110
};
        }

        CORREC(PCC,NORD1,ProcID);

        BNDRY(ProcID);

        KINETI(gl->SUM,HMAS,OMAS,ProcID);

        {	
#line 119
	while (barrier_wait(&(gl->start)) == 1);
#line 119
};

        if ((ProcID == 0) && (i >= 2)) {
            {	
#line 122
	long time(); (gl->intraend) = time(0);
#line 122
};
            gl->intratime += gl->intraend - gl->intrastart;
        }

        TKIN=TKIN+gl->SUM[0]+gl->SUM[1]+gl->SUM[2];
        TVIR=TVIR-gl->VIR;

        /*  check if potential energy is to be computed, and if
            printing and/or saving is to be done, this time step.
            Note that potential energy is computed once every NPRINT
            time-steps */

        if (((i % NPRINT) == 0) || ( (NSAVE > 0) && ((i % NSAVE) == 0))){

            if ((ProcID == 0) && (i >= 2)) {
                {	
#line 137
	long time(); (gl->interstart) = time(0);
#line 137
};
            }

            /*  call potential energy computing routine */
            POTENG(&gl->POTA,&gl->POTR,&gl->POTRF,ProcID);
            {	
#line 142
	while (barrier_wait(&(gl->start)) == 1);
#line 142
};

            if ((ProcID == 0) && (i >= 2)) {
                {	
#line 145
	long time(); (gl->interend) = time(0);
#line 145
};
                gl->intertime += gl->interend - gl->interstart;
            }

            POTA=gl->POTA*FPOT;
            POTR=gl->POTR*FPOT;
            POTRF=gl->POTRF*FPOT;

            /* compute some values to print */
            XVIR=TVIR*FPOT*0.50/TTMV;
            AVGT=TKIN*FKIN*TEMP*2.00/(3.00*TTMV);
            TEN=(gl->SUM[0]+gl->SUM[1]+gl->SUM[2])*FKIN;
            XTT=POTA+POTR+POTRF+TEN;

            if ((i % NPRINT) == 0 && ProcID == 0) {
                fprintf(six,"     %5ld %14.5lf %12.5lf %12.5lf  \
                %12.5lf\n %16.3lf %16.5lf %16.5lf\n",
                        i,TEN,POTA,POTR,POTRF,XTT,AVGT,XVIR);
            }
        }

        /* wait for everyone to finish time-step */
        {	
#line 167
	while (barrier_wait(&(gl->start)) == 1);
#line 167
};

        if ((ProcID == 0) && (i >= 2)) {
            {	
#line 170
	long time(); (gl->trackend) = time(0);
#line 170
};
            gl->tracktime += gl->trackend - gl->trackstart;
        }
    } /* for i */

    return(XTT);

} /* end of subroutine MDMAIN */
