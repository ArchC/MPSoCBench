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

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <tlm.h>
#include "tlm_memory.h"

#include "../../defines.h"

#include "DRAMSim.h"


//#include "ac_tlm_payload_extension.H"

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


//using user::tlm_payload_extension;

/// Constructor
tlm_memory::tlm_memory(sc_module_name module_name, 
		       unsigned int start_address,
		       unsigned int end_address)
    : sc_module(module_name),
      LOCAL_target_socket("LOCAL_target_socket"),
      m_start_address(start_address),
      m_end_address(end_address)
  {

	
	LOCAL_target_socket.register_b_transport(this, &tlm_memory::b_transport);

	
    sc_assert(m_start_address <= m_end_address);
    sc_assert((m_end_address-m_start_address+1)%4 == 0);
    unsigned int size = (m_end_address-m_start_address+1);
    memory = new uint8_t [size];
    for (unsigned int i = 0; i < size; ++i)
      memory[i] = 0;


    
    this->processorsReads.push_back(0);
    this->processorsWrites.push_back(0);

    #ifdef DRAMSIM2
    DRAMSim_mem = getMemorySystemInstance("../ip/tlm_memory_at/ini/DDR2_micron_16M_8b_x8_sg3E.ini", "../ip/tlm_memory_at/system.ini.example", "..", "teste", ((size/1024)/1024));
    #endif

    
  }



/// Destructor
tlm_memory::~tlm_memory() {

  if (measures) 
  {
     
     local_memory_file = fopen (LOCAL_FILE_MEASURES_NAME,"a");
     global_memory_file = fopen (GLOBAL_FILE_MEASURES_NAME,"a");


     printf("\n************************************ Memory report ************************************");
     printf("\nMemory Reads:\t%ld \nMemory Writes:\t%ld",count_read_memory, count_write_memory);
     printf("\nDetailed memory reports in the local_report.txt file.");
     
     fprintf(local_memory_file, "\n************************************ Memory report ************************************");
     fprintf(local_memory_file, "\nMemory Reads:\t%ld \nMemory Writes:\t%ld",count_read_memory, count_write_memory);
     fprintf(local_memory_file, "\nReads and writes per core:");
     fprintf(local_memory_file, "\nTotal reads\tTotal writes");
     
     for (unsigned int  it = 0, it2 = 0; it <= this->processorsReads.size(); it++, it2++)
     {
      if(this->processorsReads[it] > 0)
        fprintf(local_memory_file, "\n%u: %u", it+1, this->processorsReads[it]);
      if(this->processorsWrites[it] > 0)
        fprintf(local_memory_file, "\t%u", this->processorsWrites[it2]);
     }
     
     //fclose (local_memory_file);

     fprintf(global_memory_file, "\n************************************ Memory report ************************************");
     fprintf(global_memory_file, "\nMemory Reads:\t%ld \nMemory Writes:\t%ld",count_read_memory, count_write_memory);
     /* Impressão detalhada de reads/writes por core */
     fprintf(global_memory_file, "\nReads and writes per core:");
     fprintf(global_memory_file, "\nTotal reads\tTotal writes");
     /* O iterador é comparado somente a um vetor pois assume-se que ambos terão o mesmo tamanho */
     for (unsigned int it = 0, it2 = 0; it <= this->processorsReads.size(); it++, it2++)
     {
      if(this->processorsReads[it] > 0)
        fprintf(global_memory_file, "\n%u: %u", it+1, this->processorsReads[it]);
      if(this->processorsWrites[it] > 0)
        fprintf(global_memory_file, "\t%u", this->processorsWrites[it2]);
     }
     
    
        #ifdef DRAMSIM2
        this->printStatus(local_memory_file, true, true);
        this->printStatus(global_memory_file,true, true);
        printf("\n\nDRAMSim output:\n");
        this->printStatus(local_memory_file, true, false);
        #endif

     fclose (global_memory_file);
     fclose (local_memory_file);

  }


  #ifdef DRAMSIM2
  delete DRAMSim_mem;
  #endif

  delete [] memory;

}


#ifdef DRAMSIM2
/// Aligning memory adresses to a DRAM transaction
void tlm_memory::alignTransactionAddress(uint64_t &addr)
{
  // zero out the low order bits which correspond to the size of a transaction

  unsigned throwAwayBits = THROW_AWAY_BITS;

  addr >>= throwAwayBits;
  addr <<= throwAwayBits;
}



/// Simple print method
void tlm_memory::printStatus(FILE *file, bool finalStats, bool toFile) const
{
  this->DRAMSim_mem->printStats(file, finalStats, toFile);
}

#endif


/** Internal Write
  * Note: Always write 32 bits
  * @param a is the address to write
  * @param d id the data being write

*/

ac_tlm_rsp_status tlm_memory::writem( const uint32_t &a , const unsigned char *d, unsigned int len)
{

  char dbg[512];
  char *dbgt = dbg;
  unsigned int addr = a;
  
  

  for (unsigned int i = 0; i<len;) {
    sprintf(dbgt, "%02x", d[i]);
    dbgt += 2;
    memory[addr++] = d[i++];
  }

  
  #ifdef DRAMSIM2
   uint64_t addr2 = a;
  bool isWrite = true;
  /* TODO - casting para tipo uint64_t */
  this->alignTransactionAddress(addr2);
  TransactionType type = isWrite ? DATA_WRITE : DATA_READ;
  Transaction *trans = new Transaction(type,addr2,NULL);
  DRAMSim_mem->addTransaction(*trans);
  DRAMSim_mem->update();
  delete trans;
  #endif
 

  if(MEMORY_DEBUG)   printf("\nMEMORY WRITE: writing data--> 0x%s address--> [%#x](%u)", dbg, a, len);

  return SUCCESS;

}

/** Internal Read
  * Note: Always read 32 bits
  * @param a is the address to read
  * @param d id the data that will be read
  
*/

ac_tlm_rsp_status tlm_memory::readm( const uint32_t &a , unsigned char *d, unsigned int len)
{
	
  char dbg[512];
  char *dbgt = dbg;
  unsigned int addr = a;
  
  

  for (unsigned int i = 0; i<len;) {
    sprintf(dbgt, "%02x", memory[addr]);
    dbgt += 2;
    (((uint8_t*)d)[i++]) = memory[addr++];
  }


  #ifdef DRAMSIM2
   bool isWrite = false;
  uint64_t addr2 = a;
  /* TODO - casting para tipo uint64_t */
  this->alignTransactionAddress(addr2);
  TransactionType type = isWrite ? DATA_WRITE : DATA_READ;
  Transaction *trans = new Transaction(type,addr2,NULL);
  DRAMSim_mem->addTransaction(*trans);
  DRAMSim_mem->update();
  delete trans;
  #endif



  if(MEMORY_DEBUG)   printf("\nMEMORY READ: reading data--> 0x%s  address--> [%#x](%u)", dbg, a, len);

  
  return SUCCESS;



}


void tlm_memory::b_transport(ac_tlm2_payload& payload, sc_core::sc_time &time_info)
{
  
    time_info = time_info + sc_core::sc_time(TIME_MEMORY,SC_NS);

    uint32_t addr = (uint32_t) payload.get_address();
    unsigned char* data_pointer = payload.get_data_ptr();
    unsigned int len = payload.get_data_length();
    unsigned int procId = payload.get_streaming_width();

    tlm_command command = payload.get_command();

    if(MEMORY_DEBUG) printf("\nMEMORY TRANSPORT: command--> %d address--> %d lenght-->%u ****" , command, addr, len);


    switch( command )
    {
    	case TLM_READ_COMMAND :    
       		count_read_memory++;
       		readm( addr,data_pointer, len);
       		
       		if(procId > this->processorsReads.size())
       		{
        		this->processorsReads.resize(procId);
       		}
       		this->processorsReads[procId] = this->processorsReads[procId] + 1;
       		payload.set_response_status(tlm::TLM_OK_RESPONSE);
       		break; 
      	case TLM_WRITE_COMMAND:    
       		count_write_memory++;
       		writem (addr,data_pointer, len);
       		if(procId > this->processorsWrites.size())
       		{
        		this->processorsWrites.resize(procId);
       		}
       		
       		this->processorsWrites[procId] = this->processorsWrites[procId] + 1;
       		payload.set_response_status(tlm::TLM_OK_RESPONSE);
       		break;
      	default :
       		break; 
    }

    wait(TIME_MEMORY, SC_NS);

}

// LOADER FUNCTIONS


ac_tlm_rsp_status tlm_memory::read(unsigned int address, unsigned int size, unsigned char *data)
{
  unsigned int m_size = (m_end_address-m_start_address+1);
	unsigned int addr = address - m_start_address;

	if (addr < 0) {
    	printf("Out of bounds memory address (%d) [%d - %d].", address, m_start_address, m_end_address);
		return ERROR;
	}

	if ((addr + size) > m_size) {
    	printf("Requesting more memory than available [(%d) %d / %d].", addr, size, m_size);
		size = m_size - addr;
	}

	memcpy(data, &(memory[addr]), size);
	return SUCCESS;
}
ac_tlm_rsp_status tlm_memory::write(unsigned int address, unsigned int size, const unsigned char *data)
{
  unsigned int m_size = (m_end_address-m_start_address+1);
	unsigned int addr = address - m_start_address;

	if (addr < 0) {
		printf("Out of bounds memory address (%d) [%d - %d].", address, m_start_address, m_end_address);
		return ERROR;
	}

	if ((addr + size) > m_size) {
    printf("Requesting more memory than available [(%d) %d / %d].", addr, size, m_size);
		size = m_size - addr;
	}

	memcpy(&(memory[addr]), data, size);
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
