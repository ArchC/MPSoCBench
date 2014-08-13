/*******************************************************
 * This is the part of the MPSoCBench benchmark suite  *
 * If you want more information on MPSoCBench or ArchC,*
 * please visit:                                       *
 * http://www.archc.org/benchs/mpsocbench , or         *
 * http://www.archc.org                                *
 * Computer Systems Laboratory (LSC)                   *
 * IC-UNICAMP                                          *
 * http://www.lsc.ic.unicamp.br                        *
 ******************************************************/


/**
 * @file      tlm_memory.h
 * @author    Bruno de Carvalho Albertini
 *	      Liana Duenha
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 * 
 * @version   
 * @date      Sun, 02 Apr 2006 08:07:46 -0200
 * @brief     Defines a ac_tlm memory.
 *

 * @date      Wed, 22 Oct 2012 
 * @brief     Defines a TLM 2.0 memory
 *
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

#ifndef TLM_MEMORY_H_
#define TLM_MEMORY_H_

//////////////////////////////////////////////////////////////////////////////

// Standard includes
// SystemC includes

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc>
// ArchC includes
#include "ac_tlm_protocol.H"
#include <tlm.h>
#include "tlm_utils/simple_target_socket.h"


//////////////////////////////////////////////////////////////////////////////

// using statements
// using tlm::tlm_transport_if;

//using tlm::tlm_blocking_transport_if;


//////////////////////////////////////////////////////////////////////////////

/// Namespace to isolate memory from ArchC
namespace user
{

/// A TLM memory
class tlm_memory:
  public sc_module
{

public:

   

  tlm_utils::simple_target_socket<tlm_memory> LOCAL_target_socket;
  ac_tlm_rsp_status writem( const uint32_t & , const unsigned char * );
  ac_tlm_rsp_status readm( const uint32_t & , unsigned char*);
  
  tlm_memory( sc_module_name module_name , unsigned int i, unsigned int k); // = 536870912);

  ~tlm_memory();


  void b_transport(ac_tlm2_payload &, sc_core::sc_time &);
	

  /* memory direct access functions  - useful to load the application in memory */
	bool direct_read(int *data, unsigned int address);
	bool direct_write(int *data, unsigned int address);
	ac_tlm_rsp_status read(int *data, unsigned int address);
	ac_tlm_rsp_status write(int *data,unsigned int address);
	unsigned int start_address() const;
	unsigned int end_address() const;


private:
  uint8_t *memory;
  unsigned int m_start_address;
  unsigned int m_end_address;


};

};

#endif //TLM_MEMORY_H_
