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
 * @file      tlm_memory.h
 * @author    Liana Duenha
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

#ifndef TLM_MEMORY_H_
#define TLM_MEMORY_H_

#include <systemc>
#include "ac_tlm_protocol.H"
#include "ac_tlm2_port.H"
#include "../../defines.h"

using tlm::tlm_blocking_transport_if;

/// Namespace to isolate memory from ArchC
namespace user
{

class tlm_memory:
  public sc_module,
  public ac_tlm2_blocking_transport_if // Using ArchC TLM 2.0 protocol
{
public:

  sc_export< ac_tlm2_blocking_transport_if > target_export;
 
  tlm_memory( sc_module_name module_name , unsigned int i, unsigned int k); // = 536870912);

   /*
   * Implementation of TLM blocking transport method that
   * handle packets (ac_tlm_payload) of the protocol doing apropriate actions.
   * This method must be implemented (required by SystemC TLM).
   */
  void b_transport(ac_tlm2_payload &, sc_core::sc_time &);
  
  ~tlm_memory();

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

  ac_tlm_rsp_status writem( const uint32_t & , const unsigned char * , unsigned int);
  ac_tlm_rsp_status readm( const uint32_t & , unsigned char*, unsigned int);
};

};

#endif //TLM_MEMORY_H_
