
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

#ifndef TLM_DFS_H_
#define TLM_DFS_H_
#include <systemc>
#include "ac_module.H"
#include "ac_tlm_protocol.H"
#include "ac_tlm2_port.H"
#include "ac_tlm2_payload.H"
#include "../../defines.h"


using tlm::tlm_blocking_transport_if;



typedef struct 
{
	PROCESSOR_NAME *procPointer;
	int numberOfStates;
	int *listOfStates;
} tPowerStates;


namespace user
{


class tlm_dfs:
  public sc_module,
  public ac_tlm2_blocking_transport_if 
{
public:
   
  sc_export<ac_tlm2_blocking_transport_if> target_export;

  sc_port<ac_tlm2_blocking_transport_if> *CPU_port; //[NPROCS];


  tlm_dfs(sc_module_name module_name, int, PROCESSOR_NAME**);
  ~tlm_dfs();
  
   void b_transport(ac_tlm2_payload &, sc_core::sc_time &);
   void initializePowerStates();
   int getPowerState (int);
   void setPowerState (int, int);
   void setProcessorMode (int id, int mode);

private:
	
	int workers;
	tPowerStates *powerStates;
	PROCESSOR_NAME **processors; 


};

};

#endif //TLM_DFS_H_

#endif
