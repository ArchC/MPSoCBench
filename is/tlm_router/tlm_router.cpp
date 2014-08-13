

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
/*
 * @file      tlm_router.cpp
 * @author    Rodolfo Azevedo
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 * 
 * 
 *
 * @date      Wed, 22 Oct 2012 
 *            Modified by Liana Duenha
 * @brief     Defines a TLM 2.0 router using blocking transport method.           

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



#include "tlm_router.h"

#define measures 1

long unsigned int count_traffic = 0;
FILE *local_router_file;
FILE *global_router_file;

using user::tlm_router;


tlm_router::tlm_router( sc_module_name module_name) :
  sc_module( module_name ),
  target_export("target_export"),
  MEM_port("MEM_port", 536870912U), 			
  LOCK_port("LOCK_PORT", 0U)
  {
    target_export( *this );
  }

tlm_router::~tlm_router()
{

  if (measures) 
  {
     local_router_file = fopen (LOCAL_FILE_MEASURES_NAME,"a");
     fprintf(local_router_file, "\nRouter Access:\t%ld", count_traffic);
     fclose (local_router_file);

     global_router_file = fopen (GLOBAL_FILE_MEASURES_NAME,"a");
     fprintf(global_router_file, "\nRouter Access:\t%ld", count_traffic);
     fclose (global_router_file);

  }

 }


void tlm_router::b_transport(ac_tlm2_payload& payload, sc_core::sc_time& time_info){
    
       
    time_info = time_info + sc_core::sc_time(2,SC_NS);

    uint64_t addr = payload.get_address();
        
    if (measures) count_traffic ++;
    


    if ((addr >= 0) && (addr <BASE_MEMORY_ADDRESS))
    {
        if (ROUTER_DEBUG) { printf("\ntlm_router is transporting using MEM_port"); }
        MEM_port->b_transport(payload,time_info);

    }

    else if ((addr >= LOCK_ADDRESS)&&(addr <= LOCK_ADDRESS+DELTA_IP_ADDRESS))
	{
    	if (ROUTER_DEBUG) { printf("\ntlm_router is transporting using LOCK_port"); }

    	LOCK_port->b_transport(payload, time_info);
	}
    else
    {
    	 printf("\ntlm_router b_transport : invalid address");

    }

    if (ROUTER_DEBUG) printf("\ntlm_router b_transport returning");
      
}


