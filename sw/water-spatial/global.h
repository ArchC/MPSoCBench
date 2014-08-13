#line 156 "c.m4.null"

#line 1 "global.H"
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
#include "barrier.H"

  /* this file contains the declarations of the main data
  structure types used by the program */

#define BOTH 2
typedef double vm_type[3];

typedef struct mol_dummy {
      vm_type VM;
      double F[MXOD2][NDIR][NATOM];
} molecule_type;

typedef struct link {
      molecule_type mol;
      struct link *next_mol;
} link_type;

typedef struct box_dummy {
      struct link *list;
      mutex_t boxlock;
} box_type;

extern box_type ***BOX;

typedef struct array_dummy {
      long box[NDIR][BOTH];
} first_last_array;

extern first_last_array **start_end;

typedef struct list_of_boxes {
      long coord[3];
      struct list_of_boxes *next_box;
} box_list;

extern box_list **my_boxes;

extern double  TLC[100], FPOT, FKIN;
extern long IX[3*MXOD2+1], IRST,NVAR,NXYZ,NXV,IXF,IYF,IZF,IMY,IMZ;

extern long NumProcs;
extern long NumBoxes;

/*  This file contains the declaration of the GlobalMemory
structure and the maximum number of molecules allowed
by the program. */

struct GlobalMemory {
    mutex_t IOLock;
    mutex_t IndexLock;
    mutex_t IntrafVirLock;
    mutex_t InterfVirLock;
    mutex_t KinetiSumLock;
    mutex_t PotengSumLock;
    barrier_t start;
    barrier_t InterfBar;
    barrier_t PotengBar;
    long Index;
    double VIR;
    double SUM[3];
    double POTA, POTR, POTRF;
    unsigned long computestart,computeend;
    unsigned long trackstart, trackend, tracktime;
    unsigned long intrastart, intraend, intratime;
    unsigned long interstart, interend, intertime;
};

extern struct GlobalMemory *gl;

/* bndry.C */
void BNDRY(long ProcID);

/* cnstnt.C */
void CNSTNT(long N, double *C);

/* cshift.C */
void CSHIFT(double *XA, double *XB, double XMA, double XMB, double *XL, double BOXH, double BOXL);

/* initia.C */
void INITIA(void);
double xrand(double xl, double xh);

/* interf.C */
void INTERF(long DEST, double *VIR, long ProcID);
void UPDATE_FORCES(struct link *link_ptr, long DEST, double *XL, double *YL, double *ZL, double *FF);

/* intraf.C */
void INTRAF(double *VIR, long ProcID);

/* kineti.C */
void KINETI(double *SUM, double HMAS, double OMAS, long ProcID);

/* mdmain.C */
double MDMAIN(long NSTEP, long NPRINT, long NSAVE, long NORD1, long ProcID);

/* poteng.C */
void POTENG(double *POTA, double *POTR, double *PTRF, long ProcID);

/* predcor.C */
void PREDIC(double *C, long NOR1, long ProcID);
void CORREC(double *PCC, long NOR1, long ProcID);

/* syscons.C */
void SYSCNS(void);

/* water.C */
void WorkStart(void);


