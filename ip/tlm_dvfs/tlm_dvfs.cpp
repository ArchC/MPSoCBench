

 /********************************************************************************
	MPSoCBench Benchmark Suite
	Authors: Liana Duenha
	Supervisor: Rodolfo Azevedo
	Date: July-2012
	www.archc.org/benchs/mpsocbench

	Computer Systems Laboratory (LSC)
	IC-UNICAMP
	http://www.lsc.ic.unicamp.br/


	This source code is part of the MPSoCBench Benchmark Suite, which is a free
	source-code benchmark for evaluation of Electronic Systemc Level designs.
	This benchmark is distributed with hope that it will be useful, but
	without any warranty.

*********************************************************************************/
/**
 * @file      tlm_dvfs.h
 * @author    Liana Duenha
 *
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 * 
 * @version   0.1
 * @date      Sun, 20 Oct 2014
 * @brief     Defines a TLM 2.0 DVFS IP using blocking blocking transport method. 


 * @attention Copyright (C) 2002-2012 --- The ArchC Team
 * 
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 * 
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 * 
 * 
 */


#ifdef POWER_SIM


#include "tlm_dvfs.h"
#include <tlm.h>

#define measures 1

using user::tlm_dvfs;
using tlm::tlm_command;
using tlm::TLM_READ_COMMAND;
using tlm::TLM_WRITE_COMMAND;

long unsigned int count_dvfs = 0;
FILE *local_dvfs_file;
FILE *global_dvfs_file;

using user::tlm_dvfs;


tlm_dvfs::tlm_dvfs( sc_module_name module_name, int N_WORKERS, PROCESSOR_NAME **processors) :
  sc_module( module_name ),
  target_export("target_export")
  {
    target_export( *this );

    workers = N_WORKERS;
    powerStates = new tPowerStates [workers];

    initializePowerStates((PROCESSOR_NAME**)processors);
  
  }


tlm_dvfs::~tlm_dvfs()
{

  if (measures) 
  {
     local_dvfs_file = fopen (LOCAL_FILE_MEASURES_NAME,"a");
     fprintf(local_dvfs_file, "\ndvfs Access:\t%ld", count_dvfs);
     fclose (local_dvfs_file);

     global_dvfs_file = fopen (GLOBAL_FILE_MEASURES_NAME,"a");
     fprintf(global_dvfs_file, "\ndvfs Access:\t%ld", count_dvfs);
     printf("\ndvfs Access:\t%ld", count_dvfs);
     fclose (global_dvfs_file);

  }

  for (int i=0; i<workers; i++)
  {
    delete powerStates[i].listOfStates;
  }

  delete [] powerStates;
 }


void tlm_dvfs::b_transport(ac_tlm2_payload& payload, sc_core::sc_time& time_info)
{
    
    // forward and backward paths   
    time_info = time_info + sc_core::sc_time(time_dvfs+time_dvfs,SC_NS);

    uint32_t addr = (uint32_t) payload.get_address();
    unsigned char* d = payload.get_data_ptr();

    tlm_command command = payload.get_command();

    unsigned int procId = payload.get_streaming_width();

    if (measures) count_dvfs ++;

    switch( command )
    {
      case TLM_READ_COMMAND :    
    
          // just for test
          (((uint8_t*)d)[0]) = 0;
          (((uint8_t*)d)[1]) = 0;
          (((uint8_t*)d)[2]) = 0;
          (((uint8_t*)d)[3]) = (uint8_t) getPowerState (procId);



          if (DVFS_DEBUG)
          {
            printf("\nDVFS: tlm_dvfs received a READ request from processor %d", procId);
            printf("\nDVFS: Current Power State -> %d", *d);
          }

      
          payload.set_response_status(tlm::TLM_OK_RESPONSE);
          break; 
      case TLM_WRITE_COMMAND: 

          if (DVFS_DEBUG)
          {
            printf("\nDVFS: received a WRITE request from processor %d", procId);
            printf("\nDVFS: Current Power State -> %d", getPowerState(procId));
            printf("\nDVFS: New Required Power State-> %d", d[3]);
          }

          
          setPowerState (procId, (int) d[3]);
          payload.set_response_status(tlm::TLM_OK_RESPONSE);
          break;
      default :
          break; 
    }
        
    if (DVFS_DEBUG) printf("\ntlm_dvfs b_transport returning");
      
}



void tlm_dvfs::initializePowerStates(PROCESSOR_NAME** processors)
{

   
      if (DVFS_DEBUG) printf("\nInitializing DVFS");
     
      for (int i=0; i<workers; i++)
      {
        
        powerStates[i].procPointer = processors[i];
        powerStates[i].numberOfStates = ((powerStates[i].procPointer)->ps).getNumberOfStates();
        powerStates[i].listOfStates = new int [powerStates[i].numberOfStates];

        ((powerStates[i].procPointer)->ps).completeListOfStates(powerStates[i].listOfStates); 

          if (DVFS_DEBUG) 
          {
            printf("\nDVFS: There are %d available frequencies for Processor %d: ", powerStates[i].numberOfStates, (powerStates[i].procPointer)->getId() );
            for (int j=0; j<powerStates[i].numberOfStates; j++)
            {
              printf("%d,",powerStates[i].listOfStates[j]);
            }    
          }

          if (DVFS_DEBUG) printf("\nDVFS: Initializing Processor %d with power state %d.",i,0);

          setPowerState (i,0);


      }
   



}


int tlm_dvfs::getPowerState (int id)
{

  

    if (DVFS_DEBUG) printf("\nDVFS: getPowerState is returning the power state of processor %d", id);
    return ((powerStates[id].procPointer)->ps).getPowerState();

  
}

void tlm_dvfs::setPowerState (int id, int state)
{

  

   if (DVFS_DEBUG){
      printf("\nDVFS: setPowerState is updating the power state of processor %d", id);
      printf("\nDVFS: setPowerState -> old power state:%d\tnew power state->%d", getPowerState (id), state);

   }

   int newFrequency;
   if (state < powerStates[id].numberOfStates)
   {
         
        newFrequency = powerStates[id].listOfStates[state];
     //   powerStates[id].procPointer->set_proc_freq(newFrequency);
         if (DVFS_DEBUG)
         { 
            printf("\nDVFS: Updating processor frequency->%d",newFrequency);
         }
   }
   else
   { 
        printf("\nDVFS: The new frequency required is no availabe for this processors;");
        printf("\nDVFS: Maintaining the current frequency.");
   }    

    ((powerStates[id].procPointer)->ps).setPowerState(state);
}



#endif