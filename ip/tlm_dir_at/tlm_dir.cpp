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
 * @file      tlm_dir.cpp
 * @author    Mateus Tostes e Horst Siegmann
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
 * @brief     Implements a ac_tlm directory
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


#include "tlm_dir.h"
#include "tlm_payload_dir_extension.h"


// If you don't want to measure dir access, modify measures_dir to 0
#define measures 1

long  unsigned int count_dir = 0;
FILE *local_dir_file;
FILE *global_dir_file;

using user::tlm_dir;
using tlm::tlm_command;
using tlm::TLM_READ_COMMAND;
using tlm::TLM_WRITE_COMMAND;
using tlm::tlm_extension_base;
using user::tlm_payload_dir_extension;

tlm_dir::tlm_dir(sc_module_name module_name, int n) :
  sc_module( module_name ),
  LOCAL_target_socket("LOCAL_target_socket")
{
    /// Binds target_export to the memory
    workers = n;
    LOCAL_target_socket.register_b_transport(this, &tlm_dir::b_transport);
    CPU_port = new sc_port<ac_tlm2_blocking_transport_if>[workers];

    value = 0;
	  dir = new Dir();
	  //local_dir_file = fopen ("dir_acess.txt","a");
    
}

/// Destructor
tlm_dir::~tlm_dir() {
  
 if (measures) 
  {
     global_dir_file = fopen (GLOBAL_FILE_MEASURES_NAME,"a");
     
     fprintf(global_dir_file, "\nDir Access:\t%ld", count_dir);
     //fclose (local_dir_file);
     //fclose (global_dir_file);
  }
  delete [] CPU_port;
  delete dir;
}

bool tlm_dir::read_dir()
{
	return true;
}
bool tlm_dir::write_dir()
{
	return true;
}
uint32_t tlm_dir::readm(uint32_t &d )
{
        if(d != 0x0)
		return 0x1;

	else
		return d;

}

void tlm_dir::b_transport( ac_tlm2_payload &payload, sc_core::sc_time &time_info ) {
   
   tlm_payload_dir_extension *payloadExt;
   time_info = time_info + sc_core::sc_time(1,SC_NS);
   int nCache=-1; 
   bool validation=false;

   tlm::tlm_extension_base* base;
   base = payload.get_extension(0);

   payloadExt = reinterpret_cast<tlm_payload_dir_extension*>(base);

   if(payloadExt != 0)
   {
	    nCache = payloadExt->getnumberCache(); //id do processador
		uint32_t address =-1;
		address= payloadExt->getAddress(); //endereco do dado armazenado
		int cacheIndex = payloadExt->getCacheIndex(); //indice do vetor da cache
		int rule = payloadExt->getRule(); //comando requisitado
		if(rule == 0){
			int nWay = payloadExt->getNWay();
			int index_size = payloadExt->getIndex_size();
			dir->start(nWay, index_size);
		}
		if(rule == 1) //Cache leu dado da memoria, atualizar valor no diretorio
		{
			dir->validate(nCache, address, cacheIndex);
		}
		if(rule==2) //aconteceu escrita na memoria, invalidar todos os outros processadores diferentes de nCache
		{
			dir->unvalidate(nCache, address, cacheIndex);
		}
		if(rule == 3) //Verificar se dado da cache esta Valido
		{
			validation = dir->checkValidation(nCache, address, cacheIndex);
			payloadExt->setValidation(validation);
		}
	}
   tlm_command command = payload.get_command();
   uint32_t addr = (uint32_t) payload.get_address();
  
   unsigned char* data_pointer = payload.get_data_ptr();
  
   unsigned int len = payload.get_data_length();
   
   unsigned int resp;

   payload.set_response_status(tlm::TLM_OK_RESPONSE);    
   
   

   // TEST   
   //send(0,INTR_CACHE_INVALIDATE);
   //send(1,INTR_CACHE_INVALIDATE);

   wait (TIME_DIR, SC_NS);
   return;
    
  }


void tlm_dir::send (int id, int intr)
{
    

    if (INTR_CTRL_DEBUG) printf("\nTLM_INTR_CTRL: sending to processor %d an interruption with code %d", id, intr);
    sc_core::sc_time time_info = sc_time_stamp();
    unsigned char p[4];

    uint32_t *T = reinterpret_cast<uint32_t*>(p);
    T[0] = intr;
    
    payload.set_command(tlm::TLM_WRITE_COMMAND);
    payload.set_data_ptr(p);

    
    //payload.set_address(50);

    CPU_port[id]->b_transport(payload, time_info); 
 
}


