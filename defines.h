/******************************************************************************
 * @file      defines.h
 * @author    Liana Duenha
 *
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 * 
 * @date      01, Feb, 2013
 * @brief     Defines the ArchC TLM 2.0  port.

 *
 * @attention Copyright (C) 2002-2012 --- The ArchC Team
 *
 *******************************************************************************/


#ifndef _DEFINES_H 
#define _DEFINES_H

// PLATFORM STATIC PARAMETERS
#define LOCAL_FILE_MEASURES_NAME "local_report.txt"
#define GLOBAL_FILE_MEASURES_NAME "../../global_report.txt"

#define MAX_WORKERS 64 	/* This is a arbitrary limit for the number of processors. If necessary, this value can be modified, but
 there is no guarantee that all applications will work properly */

// NOC and Router STATIC PARAMETERS
#define MAX_LINES 72
#define MAX_COLUMNS 3 
#define ON 1
#define OFF 0
#define SIZE_OF_BUFFER_PACKAGES 2000

 // These parameters define the address space for IPs
// We consider that memory address space is from 0 to 0x20000000-1
// The address to access the Lock IP is 0x20000000
// The address to access the DVFS IP is 0x21000000
// The address to access the INTR_CTRL IP is 0x22000000
// The address to access the DIR IP is 0x23000000
// Next, we use a DELTA_IP_ADDRESS = 0x100 as a default value
// If you want to update these values, be carefull maintaining the coherency with the tlm_router, tlm_noc_at and tlm_noc_lt
// routing definitions (see is/tlm_router, is/tlm_noc_lt, is/tlm_noc_at)
#define BASE_MEMORY_ADDRESS 536870912 // 0x20000000 -  last memory byte address is 536870911

#define LOCK_ADDRESS        0x20000000
#define INTR_CTRL_ADDRESS   0X21000000
#define DIR_ADDRESS         0x22000000
#define DFS_ADDRESS         0x23000000
#define DELTA_IP_ADDRESS    0x00000100
#define DELTA               16777216  // 16^6 = 0x1000000

//#define DRAMSIM2

// DEBUG FLAGS
#define NOC_DEBUG       0
#define LOCK_DEBUG      0
#define MEMORY_DEBUG    0
#define ROUTER_DEBUG    0
#define DFS_DEBUG       0
#define INTR_CTRL_DEBUG 0

// ROUTER STATIC PARAMETERS

// TLM2 PAYLOAD STATIC PARAMETERS
// Define the transaction direction
#define FORWARD  0
#define BACKWARD 1

// TIMING STATIC PARAMETERS
#define TIME_MEMORY    100
#define TIME_DIR       20
#define TIME_NODE      6
#define TIME_ROUTER    20
#define TIME_LOCK      20
#define TIME_DFS       20
#define TIME_INTR_CTRL 20


// DVFS STATIC PARAMETERS
// The definition of the power states depends on how the software (application) will explore this feature
// By default, the acPthread will use just the states LOW and HIGH. So, if you want to enable DVFS with this two 
// power states, you must define the "id" that you want for each one of this states (based on the power table that you will use 
// see processor/mips/ac_power_stats.H or processors/sparc/ac_power_stats.H to know more about this)
// In case you define different power states here, you must also update the acPthread library to use them (sw/acPthread.c).

// But if you want to desable DFS (using just a single power state), use HIGH=LOW=0 


#define HIGH 3
#define LOW 0
#define INITIAL_PW_STATE 0

//#define DFS_AUTO_SELECTION_CPU_RATE
//#define DFS_AUTO_SELECTION_ENERGY_STAMP
#define FIRST_DELTA_T 0.025   // 50*0,0005
#define DELTA_T 0.00025     
#define N_DELTA_T 50

#ifdef PROCMIPS 
    #ifdef POWER_SIM
        #undef POWER_SIM
        #define POWER_SIM "../../processors/mips/powersc" 
    #endif
    #include "../../processors/mips/mips.H"   
    #define PROCESSOR_NAME mips
	#define PROCESSOR_NAME_parms mips_parms
#endif 

#ifdef PROCSPARC
    #ifdef POWER_SIM
        #undef POWER_SIM
        #define POWER_SIM "../../processors/sparc/powersc" 
    #endif
    #include "../../processors/sparc/sparc.H"
	#define PROCESSOR_NAME sparc
	#define PROCESSOR_NAME_parms sparc_parms
#endif 

#ifdef PROCPOWERPC
    #ifdef POWER_SIM
        #undef POWER_SIM
        #define POWER_SIM "../../processors/powerpc/powersc" 
    #endif
    #include "../../processors/powerpc/powerpc.H"
	#define PROCESSOR_NAME powerpc
	#define PROCESSOR_NAME_parms powerpc_parms
#endif 

#ifdef PROCARM
    #ifdef POWER_SIM
        #undef POWER_SIM
        #define POWER_SIM "../../processors/arm/powersc" 
    #endif
    #include "../../processors/arm/arm.H"
	#define PROCESSOR_NAME arm
	#define PROCESSOR_NAME_parms arm_parms
#endif 

// NoC Hermes STATIC PARAMETERS - NOT IN USE IN THIS VERSION
#define LOCAL_MEM_SIZE  67108864
#define TEXTAREA	0x00d16f
#endif
