/********************************************************************************
	MPSoCBench Benchmark Suite
	Authors: Liana Duenha
	Supervisor: Rodolfo Azevedo
	Date: July-2012
	

	Computer Systems Laboratory (LSC)
	IC-UNICAMP
	http://www.lsc.ic.unicamp.br/
	
	
	This source code is part of the MPSoCBench Benchmark Suite, which is a free
	source-code benchmark for evaluation of Electronic Systemc Level designs.
	This benchmakr is distributed int he hope that it will be useful, but 
	without any warranty.
 
*********************************************************************************/

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

/*  Usage:   water < infile,
    where infile has 10 fields which can be described in order as
    follows:

    TSTEP:   the physical time interval (in sec) between timesteps.
    Good default is 1e-15.
    NMOL:    the number of molecules to be simulated.
    NSTEP:   the number of timesteps to be simulated.
    NORDER:  the order of the predictor-corrector method to be used.
    set this to 6.
    NSAVE:   the frequency with which to save data in data collection.
    Set to 0 always.
    NRST:    the frequency with which to write RST file: set to 0 always (not used).
    NPRINT:  the frequency with which to compute potential energy.
    i.e. the routine POTENG is called every NPRINT timesteps.
    It also computes intermolecular as well as intramolecular
    interactions, and hence is very expensive.
    NFMC:    Not used (historical artifact).  Set to anything, say 0.
    NumProcs: the number of processors to be used.
    CUTOFF:  the cutoff radius to be used (in Angstrom,
    floating-point).  In a real simulation, this
    will be set to 0 here in which case the program will
    compute it itself (and set it to about 11 Angstrom.
    It can be set by the user if they want
    to use an artificially small cutoff radius, for example
    to control the number of boxes created for small problems
    (and not have fewer boxes than processors).
    */

MAIN_ENV
#include <stdio.h>
#include <string.h>
#include "split.h"

/*  include files for declarations  */
#include "parameters.h"
#include "mdvar.h"
#include "water.h"
#include "wwpot.h"
#include "cnst.h"
#include "mddata.h"
#include "fileio.h"
#include "frcnst.h"
#include "global.h"

#define PROC 1

long NMOL,NORDER,NATMO,NATMO3,NMOL1;
long NATOMS;
long I2;

double TLC[100], FPOT, FKIN;
double TEMP,RHO,TSTEP,BOXL,BOXH,CUTOFF,CUT2;
double R3[128],R1;
double UNITT,UNITL,UNITM,BOLTZ,AVGNO,PCC[11];
double OMAS,HMAS,WTMOL,ROH,ANGLE,FHM,FOM,ROHI,ROHI2;
double QQ,A1,B1,A2,B2,A3,B3,A4,B4,AB1,AB2,AB3,AB4,C1,C2,QQ2,QQ4,REF1,REF2,REF4;
double FC11,FC12,FC13,FC33,FC111,FC333,FC112,FC113,FC123,FC133,FC1111,FC3333,FC1112,FC1122,FC1113,FC1123,FC1133,FC1233,FC1333;

//FILE *six;

molecule_type *VAR;

struct GlobalMemory *gl;        /* pointer to the Global Memory
                                   structure, which contains the lock,
                                   barrier, and some scalar variables */

long NSTEP, NSAVE, NRST, NPRINT,NFMC;
long NORD1;
long II;                         /*  variables explained in common.h */
long i;
long NDATA;
long NFRST=11;
long NFSV=10;
long LKT=0;

long StartMol[MAXPROCS+1];       /* number of the first molecule
                                   to be handled by this process; used
                                   for static scheduling     */
long MolsPerProc;                /* number of mols per processor */
long NumProcs;                   /* number of processors being used;
                                   run-time input           */
double XTT;


unsigned volatile int procCounter = 0;
FILE *six;

extern unsigned volatile int *LOCKER; // = (unsigned int *)0x20000000;


int main0(int argc, char *argv[]);

int main(int argc, char *argv[])
{
  register int procNumber;

  AGlobalLock();
      procNumber = procCounter++;
  RGlobalLock();
  
  if (procNumber == 0)
  {	
  
	printf("\n");
        printf("\n");
       	printf("--------------------------------------------------------------------\n");
	printf("--------------------------  MPSoCBench  ----------------------------\n");
       	printf("------------------------- Running: water ---------------------------\n");
       	printf("--------------- The results will be available in -------------------\n");
       	printf("------------------------ the output file ---------------------------\n");
       	printf("--------------------------------------------------------------------\n");
	printf("\n");

	/* OUTPUT FILE */
	six = fopen("output","w");
	if (six == NULL){
	    printf("Error: fopen()\n");
	    exit(1);
	}

	main0(argc, argv);
	fclose(six);

  } else{
      WorkStart();
  }

  MAIN_END;	

}
//=============================================

int main0(int argc, char **argv)
{
    /* default values for the control parameters of the driver */
    /* are in parameters.h */

    if ((argc == 2) &&((strncmp(argv[1],"-h",strlen("-h")) == 0) || (strncmp(argv[1],"-H",strlen("-H")) == 0))) {
        printf("Usage:  WATER-NSQUARED < infile, where the contents of infile can be\nobtained from the comments at the top of water.C and the first scanf \nin main() in water.C\n\n");
        exit(0);
    }

    /*  POSSIBLE ENHANCEMENT:  Here's where one might bind the main process
        (process 0) to a processor if one wanted to. Others can be bound in
        the WorkStart routine.
        */

    //six = stdout;   /* output file */

    TEMP  =298.0;
    RHO   =0.9980;
    CUTOFF=0.0;

    /* read input */

    TSTEP    = 1.5e-16;
    NMOL     = 27; //8; //27;	// 64 Number of Molecules must be an integer cube (8, 27, 64, 343, 512 ...)
    NSTEP    = 1;   // 3 originalmente // 2
    NORDER   = 6;
    NSAVE    = -1;
    NRST     = 3000;
    NPRINT   = 3;
    NFMC     = 0;
    // NumProcs = PROC;	//Number of Processors
    // NumProcs = atoi (argv[1]);
    //NumProcs = NPROC;
    NumProcs = NPROC;

    CUTOFF   = 6.212752;

    //if (scanf("%lf%ld%ld%ld%ld%ld%ld%ld%ld%lf",&TSTEP, &NMOL, &NSTEP, &NORDER,
    //          &NSAVE, &NRST, &NPRINT, &NFMC,&NumProcs, &CUTOFF) != 10)
    //    fprintf(stderr,"ERROR: Usage: water < infile, which must have 10 fields, see SPLASH documentation or comment at top of water.C\n");

    if (NMOL > MAXLCKS) {
        fprintf(stderr, "Just so you know ... Lock array in global.H has size %ld < %ld (NMOL)\n code will still run correctly but there may be lock contention\n\n", MAXLCKS, NMOL);
    }

    printf("Using %ld procs on %ld steps of %ld mols\n", NumProcs, NSTEP, NMOL);
    printf("Other parameters:\n\tTSTEP = %8.2e\n\tNORDER = %ld\n\tNSAVE = %ld\n",TSTEP,NORDER,NSAVE);
    printf("\tNRST = %ld\n\tNPRINT = %ld\n\tNFMC = %ld\n\tCUTOFF = %lf\n\n",NRST,NPRINT,NFMC,CUTOFF);


    fprintf(six,"Using %ld procs on %ld steps of %ld mols\n", NumProcs, NSTEP, NMOL);
    fprintf(six,"Other parameters:\n\tTSTEP = %8.2e\n\tNORDER = %ld\n\tNSAVE = %ld\n",TSTEP,NORDER,NSAVE);
    fprintf(six,"\tNRST = %ld\n\tNPRINT = %ld\n\tNFMC = %ld\n\tCUTOFF = %lf\n\n",NRST,NPRINT,NFMC,CUTOFF);

    /* SET UP SCALING FACTORS AND CONSTANTS */

    NORD1=NORDER+1;

    CNSTNT(NORD1,TLC);  /* sub. call to set up constants */


    { /* Do memory initializations */
        long pid;
        long mol_size = sizeof(molecule_type) * NMOL;
        long gmem_size = sizeof(struct GlobalMemory);

        /*  POSSIBLE ENHANCEMENT:  One might bind the first process to
            a processor here, even before the other (child) processes are
            bound later in mdmain().
            */

        MAIN_INITENV(,70000000,);  /* macro call to initialize
                                      shared memory etc. */

        /* allocate space for main (VAR) data structure as well as
           synchronization variables */

        /*  POSSIBLE ENHANCEMENT: One might want to allocate a process's
            portion of the VAR array and what it points to in its local
            memory */

        VAR = (molecule_type *) G_MALLOC(mol_size);
        gl = (struct GlobalMemory *) G_MALLOC(gmem_size);

        /*  POSSIBLE ENHANCEMENT: One might want to allocate  process i's
            PFORCES[i] array in its local memory */

        PFORCES = (double ****) G_MALLOC(NumProcs * sizeof (double ***));
        { long i,j,k;

          for (i = 0; i < NumProcs; i++) {
              PFORCES[i] = (double ***) G_MALLOC(NMOL * sizeof (double **));
              for (j = 0; j < NMOL; j++) {
                  PFORCES[i][j] = (double **) G_MALLOC(NDIR * sizeof (double *));
                  for (k = 0; k < NDIR; k++) {
                      PFORCES[i][j][k] = (double *) G_MALLOC(NATOM * sizeof (double));
                  }
              }
          }
      }
        /* macro calls to initialize synch varibles  */

        BARINIT(gl->start, NumProcs);
	BARINIT(gl->InterfBar, NumProcs);
	BARINIT(gl->PotengBar, NumProcs);
        LOCKINIT(gl->IOLock);
        LOCKINIT(gl->IndexLock);
        LOCKINIT(gl->IntrafVirLock);
        LOCKINIT(gl->InterfVirLock);
        LOCKINIT(gl->FXLock);
        LOCKINIT(gl->FYLock);
        LOCKINIT(gl->FZLock);
        if (NMOL < MAXLCKS) {
            ALOCKINIT(gl->MolLock, NMOL);
        }
        else {
            ALOCKINIT(gl->MolLock, MAXLCKS);
        }
        LOCKINIT(gl->KinetiSumLock);
        LOCKINIT(gl->PotengSumLock);

	join_init(&myJoinPoint, NumProcs);

        /* set up control for static scheduling */

        MolsPerProc = NMOL/NumProcs;
        StartMol[0] = 0;
        for (pid = 1; pid < NumProcs; pid += 1) {
            StartMol[pid] = StartMol[pid-1] + MolsPerProc;
        }
        StartMol[NumProcs] = NMOL;
    }

    SYSCNS();    /* sub. call to initialize system constants  */

    fprintf(six,"\nTEMPERATURE                = %8.2f K\n",TEMP);
    fprintf(six,"DENSITY                    = %8.5f G/C.C.\n",RHO);
    fprintf(six,"NUMBER OF MOLECULES        = %8ld\n",NMOL);
    fprintf(six,"NUMBER OF PROCESSORS       = %8ld\n",NumProcs);
    fprintf(six,"TIME STEP                  = %8.2e SEC\n",TSTEP);
    fprintf(six,"ORDER USED TO SOLVE F=MA   = %8ld \n",NORDER);
    fprintf(six,"NO. OF TIME STEPS          = %8ld \n",NSTEP);
    fprintf(six,"FREQUENCY OF DATA SAVING   = %8ld \n",NSAVE);
    fprintf(six,"FREQUENCY TO WRITE RST FILE= %8ld \n",NRST);
    fprintf(six,"SPHERICAL CUTOFF RADIUS    = %8.4f ANGSTROM\n",CUTOFF);
    fflush(six);


    /* initialization routine; also reads displacements and
       sets up random velocities*/
    INITIA();

    /*.....start molecular dynamic loop */

    gl->tracktime = 0;
    gl->intratime = 0;
    gl->intertime = 0;

    /* initialize Index to 1 so that the first created child gets
       id 1, not 0 */

    gl->Index = 1;

    if (NSAVE > 0)  /* not true for input decks provided */
	fprintf(six,"COLLECTING X AND V DATA AT EVERY %4ld TIME STEPS \n",NSAVE);

    
    CLOCK(gl->computestart);
    
    printf("...\n");
    WorkStart();

    printf("...\n");
    /* macro to make main process wait for all others to finish */
    WAIT_FOR_END(myJoinPoint, NumProcs);
    CLOCK(gl->computeend);

    //fprintf(six,"COMPUTESTART (after initialization) = %lu\n",gl->computestart);
    //fprintf(six,"COMPUTEEND = %lu\n",gl->computeend);
    //fprintf(six,"COMPUTETIME (after initialization) = %lu\n",gl->computeend-gl->computestart);
    //fprintf(six,"Measured Time (2nd timestep onward) = %lu\n",gl->tracktime);
    //fprintf(six,"Intramolecular time only (2nd timestep onward) = %lu\n",gl->intratime);
    //fprintf(six,"Intermolecular time only (2nd timestep onward) = %lu\n",gl->intertime);
    //fprintf(six,"Other time (2nd timestep onward) = %lu\n",gl->tracktime - gl->intratime - gl->intertime);

    fprintf(six,"\nExited Happily with XTT = %g (note: XTT value is garbage if NPRINT > NSTEP)\n", XTT);

    
} /* main.c */

void WorkStart() /* routine that each created process starts at;
                    it simply calls the timestep routine */
{
    long ProcID;
    double LocalXTT;

    BARRIER(gl->start, NumProcs);

    LOCK(gl->IndexLock);
    ProcID = gl->Index++;
    UNLOCK(gl->IndexLock);

    BARINCLUDE(gl->start);
    BARINCLUDE(gl->InterfBar);
    BARINCLUDE(gl->PotengBar);

    ProcID = ProcID % NumProcs;

    /*  POSSIBLE ENHANCEMENT:  Here's where one might bind processes
        to processors if one wanted to.
        */
   

    LocalXTT = MDMAIN(NSTEP,NPRINT,NSAVE,NORD1,ProcID);
    if (ProcID == 0) {
	    XTT = LocalXTT;
    }
    
    join_point(&myJoinPoint);
    
}
