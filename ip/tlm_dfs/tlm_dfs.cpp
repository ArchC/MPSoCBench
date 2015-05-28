

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
 * @file      tlm_dfs.h
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
 * @brief     Defines a TLM 2.0 DFS IP using blocking blocking transport method. 


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

#include "tlm_dfs.h"
#include <tlm.h>

#define measures 1

using user::tlm_dfs;
using tlm::tlm_command;
using tlm::TLM_READ_COMMAND;
using tlm::TLM_WRITE_COMMAND;

long unsigned int count_dfs = 0;
FILE *local_dfs_file;
FILE *global_dfs_file;

using user::tlm_dfs;


tlm_dfs::tlm_dfs( sc_module_name module_name, int N_WORKERS, PROCESSOR_NAME **processors) :
  sc_module( module_name ),
  target_export("target_export")
  {

    this->processors = processors; // shalow copy
    target_export( *this );

    workers = N_WORKERS;
    powerStates = new tPowerStates [workers];

    initializePowerStates();
   
  }


tlm_dfs::~tlm_dfs()
{

  if (measures) 
  {
     local_dfs_file = fopen (LOCAL_FILE_MEASURES_NAME,"a");
     fprintf(local_dfs_file, "\ndfs Access:\t%ld", count_dfs);
     fclose (local_dfs_file);

     global_dfs_file = fopen (GLOBAL_FILE_MEASURES_NAME,"a");
     fprintf(global_dfs_file, "\ndfs Access:\t%ld", count_dfs);
     printf("\ndfs Access:\t%ld", count_dfs);
     fclose (global_dfs_file);

  }

  for (int i=0; i<workers; i++)
  {
    delete powerStates[i].listOfStates;
  }

  delete [] powerStates;
 }


void tlm_dfs::b_transport(ac_tlm2_payload& payload, sc_core::sc_time& time_info)
{
    int temp;
    // forward and backward paths   
    time_info = time_info + sc_core::sc_time(TIME_DFS,SC_NS);

    uint32_t addr = (uint32_t) payload.get_address();
    unsigned char* d = payload.get_data_ptr();
    

    tlm_command command = payload.get_command();

    unsigned int procId = payload.get_streaming_width();
    printf("\ntml_dfs transport: procId=%d", procId);

    if (measures) count_dfs ++;


    uint32_t *T = reinterpret_cast<uint32_t*>(d);

    switch( command )
    {
      case TLM_READ_COMMAND :    
    
          T[0] = getPowerState(procId);

          if (DFS_DEBUG)
          {
            printf("\nDFS: tlm_dfs received a READ request from processor %d", procId);
            printf("\nDFS: Current Power State -> %d", T[0]); //*d);
          }

      
          payload.set_response_status(tlm::TLM_OK_RESPONSE);
          break; 
      case TLM_WRITE_COMMAND: 

          temp = (int) T[0];

          #ifdef AC_GUEST_BIG_ENDIAN
          temp = be32toh(temp);
          #endif

          if (DFS_DEBUG)
          {
            printf("\nDFS: received a WRITE request from processor %d", procId);
            printf("\nDFS: Current Power State -> %d", getPowerState(procId));
            printf("\nDFS: New Required Power State -> %d", temp);
          }

          setPowerState (procId, temp);
          payload.set_response_status(tlm::TLM_OK_RESPONSE);
          break;
      default :
          break; 
    }
        
    if (DFS_DEBUG) printf("\ntlm_dfs b_transport returning");
      
}



void tlm_dfs::initializePowerStates()
{

      
      if (DFS_DEBUG) printf("\nInitializing DFS");
     
      for (int i=0; i<workers; i++)
      {
        

        powerStates[i].procPointer = processors[i];
        powerStates[i].numberOfStates = ((powerStates[i].procPointer)->ps).getNumberOfStates();
        powerStates[i].listOfStates = new int [powerStates[i].numberOfStates];


       
        ((powerStates[i].procPointer)->ps).completeListOfStates(powerStates[i].listOfStates); 
        

          if (DFS_DEBUG) 
          {
            printf("\nDFS: There are %d available frequencies for Processor %d: ", powerStates[i].numberOfStates, (powerStates[i].procPointer)->getId() );
            for (int j=0; j<powerStates[i].numberOfStates; j++)
            {
              printf("%d,",powerStates[i].listOfStates[j]);
            }    
          }
      }
      
  }

// POWER CONTROL
int tlm_dfs::getPowerState (int id)
{


   
    if (DFS_DEBUG) printf("\nDFS: getPowerState is returning the power state of processor %d", id);
    return ((powerStates[id].procPointer)->ps).getPowerState();
   

}

void tlm_dfs::setPowerState (int id, int state)
{  

   if (DFS_DEBUG){
      printf("\nDFS: setPowerState is updating the power state of processor %d", id);
      printf("\nDFS: setPowerState -> old power state:%d\tnew power state->%d", getPowerState (id), state);
   }

   int newFrequency;
   if (state < powerStates[id].numberOfStates)
   {
         
        newFrequency = powerStates[id].listOfStates[state];

         if (DFS_DEBUG)
         { 
            printf("\nDFS: Updating processor frequency->%d",newFrequency);
         }
   }
   else
   { 
        printf("\nDFS: The new frequency required is no availabe for this processors;");
        printf("\ndfs: Maintaining the current frequency.");
   }    

   ((powerStates[id].procPointer)->ps).setPowerState(state);

 
}


#endif

