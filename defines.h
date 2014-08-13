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

// PLATFORM DEFINES
#define LOCAL_FILE_MEASURES_NAME "local_report.txt"
#define GLOBAL_FILE_MEASURES_NAME "../../global_report.txt"

//#define MEM_SIZE 536870912  // 512MB

#define MAX_WORKERS 64 	/* This is a arbitrary limit for the number of processors. If necessary, this value can be modified, but
 there is no guarantee that all applications will work properly */

// NOC DEFINES
#define BASE_MEMORY_ADDRESS 536870912
#define DELTA 16777216

#define MAX_LINES 72
#define MAX_COLUMNS   3 

#define ON 1
#define OFF 0

#define NOC_DEBUG 0
#define LOCK_DEBUG 0
#define MEMORY_DEBUG 0
#define ROUTER_DEBUG 0

#define LOCAL_MEM_SIZE  67108864

#define SIZE_OF_BUFFER_PACKAGES 2000


// ROUTER DEFINES
#define LOCK_ADDRESS       0x20000000
#define DELTA_IP_ADDRESS   0X00000100
#define TEXTAREA	0x00d16f

// payload defines
#define FORWARD 0
#define BACKWARD 1

#endif
