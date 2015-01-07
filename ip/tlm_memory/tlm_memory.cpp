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
 * @file      tlm_memory.cpp
 * @author    Bruno de Carvalho Albertini
 *            Liana Duenha
 * 
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 * 
 * @version   0.1
 * @date      Sun, 02 Apr 2006 08:07:46 -0200
 * @brief     Implements a ac_tlm memory.
 *
 * @date      Wed, 30 Oct 2012 
 * @brief     Implements a TLM 2.0 memory.
 
 * @attention Copyright (C) 2002-2005 --- The ArchC Team
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

//////////////////////////////////////////////////////////////////////////////
// Standard includes
// SystemC includes
// ArchC includes

#include "tlm_memory.h"
#include <tlm.h>


//////////////////////////////////////////////////////////////////////////////
// If you want to measure memory access (read/write), turn measures to 1

#define measures 1

long int count_read_memory = 0;
long int count_write_memory = 0;
FILE *local_memory_file;
FILE *global_memory_file;

//////////////////////////////////////////////////////////////////////////////

/// Namespace to isolate memory from ArchC
using user::tlm_memory;
using tlm::tlm_command;
using tlm::TLM_READ_COMMAND;
using tlm::TLM_WRITE_COMMAND;

/// Constructor
tlm_memory::tlm_memory( sc_module_name module_name , 
                        unsigned int start_address,
		        unsigned int end_address) :
  sc_module( module_name ),
  m_start_address(start_address),
  m_end_address(end_address),
  target_export("iport")
{
    /// Binds target_export to the memory
    target_export( *this );

    sc_assert(m_start_address <= m_end_address);
    sc_assert((m_end_address-m_start_address+1)%4 == 0);
    unsigned int size = (m_end_address-m_start_address+1);

    memory = new uint8_t [size];
    for (unsigned int i = 0; i < size; ++i)
      memory[i] = 0;

}

/// Destructor
tlm_memory::~tlm_memory() {

  if (measures) 
  {
     local_memory_file = fopen (LOCAL_FILE_MEASURES_NAME,"a");
     fprintf(local_memory_file, "\nMemory Reads:\t%ld \nMemory Writes:\t%ld",count_read_memory, count_write_memory);
     fclose (local_memory_file);

     global_memory_file = fopen (GLOBAL_FILE_MEASURES_NAME,"a");
     fprintf(global_memory_file, "\nMemory Reads:\t%ld \nMemory Writes:\t%ld",count_read_memory, count_write_memory);
     printf("\nMemory Reads:\t%ld \nMemory Writes:\t%ld",count_read_memory, count_write_memory);
     fclose (global_memory_file);

  }

  delete [] memory;

}

/** Internal Write
  * Note: Always write 32 bits
  * @param a is the address to write
  * @param d id the data being write
  
*/

//ac_tlm_rsp_status tlm_memory::writem( const uint32_t &a , uint32_t d)
ac_tlm_rsp_status tlm_memory::writem( const uint32_t &a , const unsigned char *d)
{

  if(measures) count_write_memory ++;

  memory[a]   = d[0];
  memory[a+1] = d[1];
  memory[a+2] = d[2];
  memory[a+3] = d[3];


  
 
  if(MEMORY_DEBUG)  printf("\nMEMORY WRITE: writing data--> %d %d %d %d  address--> %d %d %d %d ",memory[a], memory[a+1], memory[a+2], memory[a+3],a,a+1,a+2,a+3);


  return SUCCESS;
}

/** Internal Read
  * Note: Always read 32 bits
  * @param a is the address to read
  * @param d id the data that will be read
  
*/

ac_tlm_rsp_status tlm_memory::readm( const uint32_t &a , unsigned char *d)
{
 
  if(measures) count_read_memory ++;
  
  (((uint8_t*)d)[0]) = memory[a];
  (((uint8_t*)d)[1]) = memory[a+1];
  (((uint8_t*)d)[2]) = memory[a+2];
  (((uint8_t*)d)[3]) = memory[a+3];

 
  if(MEMORY_DEBUG)  printf("\nMEMORY READ: reading data--> %d %d %d %d  address--> %d %d %d %d ",memory[a], memory[a+1], memory[a+2], memory[a+3],a,a+1,a+2,a+3);

  return SUCCESS;
}


void tlm_memory::b_transport(ac_tlm2_payload& payload, sc_core::sc_time &time_info)
{
  
  
    time_info = time_info + sc_core::sc_time(1,SC_NS);

    uint32_t addr = (uint32_t) payload.get_address();
    unsigned char* data_pointer = payload.get_data_ptr();
    
    tlm_command command = payload.get_command();

    if(MEMORY_DEBUG) printf("\nMEMORY TRANSPORT: command--> %d  address--> %d" , command, addr);

    //if (addr > MEM_SIZE) printf("\nMEMORY TRANSPORT WARNING: address %d out of limits...MEM_SIZE=%d", addr,MEM_SIZE);
   
    switch( command )
    {
    	case TLM_READ_COMMAND :    
 	     
	     readm( addr,data_pointer);
	     payload.set_response_status(tlm::TLM_OK_RESPONSE);
 	     break; 
    	case TLM_WRITE_COMMAND:    
 	     
	     writem (addr,data_pointer);
 	     payload.set_response_status(tlm::TLM_OK_RESPONSE);
	     break;
    	default :
	     break; 
    }
    if(MEMORY_DEBUG) printf("\ntlm_memory b_transport returning...");

}



// LOADER FUNCTIONS

/* memory direct access functions  - useful to load the application in memory */
bool tlm_memory::direct_read(int *data, unsigned int address)
{
	return (read(data, address) == SUCCESS);
}

bool tlm_memory::direct_write(int *data, unsigned int address)
{
	return (write(data, address) == SUCCESS);
}
ac_tlm_rsp_status tlm_memory::read(int *data, unsigned int address)
{
	//Modified to accept non aligned access
 	 ((char*)data)[0] = memory[(address - m_start_address)];
	  ((char*)data)[1] = memory[(address - m_start_address+1)];
	  ((char*)data)[2] = memory[(address - m_start_address+2)];
	  ((char*)data)[3] = memory[(address - m_start_address+3)];

	  if(MEMORY_DEBUG)   printf("\nLOADER MEMORY READ: reading data--> %d %d %d %d  address--> %d %d %d %d ",memory[address - m_start_address], memory[address - m_start_address+1], memory[address - m_start_address+2], memory[address - m_start_address+3],address - m_start_address,address - m_start_address+1,address - m_start_address+2,address - m_start_address+3);

	return SUCCESS;	  
}
ac_tlm_rsp_status tlm_memory::write(int *data,unsigned int address)
{
	//Modified to accept non aligned access
	memory[(address - m_start_address)] = ((char*)data)[0];
	memory[(address - m_start_address+1)] = ((char*)data)[1];
	memory[(address - m_start_address+2)] = ((char*)data)[2];
	memory[(address - m_start_address+3)] = ((char*)data)[3];

	if(MEMORY_DEBUG)  printf("\nLOADER MEMORY WRITE: writing data--> %d %d %d %d  address--> %d %d %d %d ",memory[address - m_start_address], memory[address - m_start_address+1], memory[address - m_start_address+2], memory[address - m_start_address+3],address - m_start_address,address - m_start_address+1,address - m_start_address+2,address - m_start_address+3);


	return SUCCESS;
}
unsigned int tlm_memory::start_address() const
{
  return m_start_address;
}

unsigned int tlm_memory::end_address() const
{
  return m_end_address;
}


 
