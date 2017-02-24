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

#ifndef TLM_DIRETORIO_H_
#define TLM_DIRETORIO_H_

#include <systemc>
#include "ac_tlm_protocol.H"
#include "ac_tlm2_port.H"
#include "../../defines.h"
#include "Diretorio.h"


using tlm::tlm_blocking_transport_if;

/// Namespace to isolate memory from ArchC
namespace user
{

class tlm_diretorio :
  public sc_module,
  public ac_tlm2_blocking_transport_if // Using ArchC TLM protocol
{
public:

   sc_export< ac_tlm2_blocking_transport_if > target_export;	
   
   uint32_t readm (uint32_t & );
   void b_transport(ac_tlm2_payload &, sc_core::sc_time &);
   bool read_dir();
   bool write_dir();
   tlm_diretorio( sc_module_name module_name);
   ~tlm_diretorio();

private:
	Diretorio *dir;
   uint32_t value;

};

};

#endif //tlm_lock_H_
