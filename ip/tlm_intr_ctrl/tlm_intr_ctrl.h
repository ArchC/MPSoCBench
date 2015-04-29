
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
 * @file      tlm_intr_ctrl.h
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
 * @brief     Defines a TLM 2.0 Interruption Controller

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




#ifndef TLM_INTR_CTRL_H_
#define TLM_INTR_CTRL_H_
#include <systemc>
#include "ac_module.H"
#include "ac_tlm_protocol.H"
#include "ac_tlm2_port.H"
#include "ac_tlm2_payload.H"
#include "../../defines.h"


using tlm::tlm_blocking_transport_if;



namespace user
{


class tlm_intr_ctrl:
  public sc_module,
  public ac_tlm2_blocking_transport_if 
{
public:
   
  sc_export<ac_tlm2_blocking_transport_if> target_export;

  sc_port<ac_tlm2_blocking_transport_if> *CPU_port; //[NPROCS];

  tlm_intr_ctrl(sc_module_name module_name,int);
  ~tlm_intr_ctrl();
  
   void b_transport(ac_tlm2_payload &, sc_core::sc_time &);
   void turnOnProcessors(int);
   void turnOffProcessors(int);
   void send(int, int);
   void send (int id, unsigned char intr);

private:
	
	int workers;
	ac_tlm2_payload payload; 



};

};



#endif
