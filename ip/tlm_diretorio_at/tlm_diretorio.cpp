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


#include "tlm_diretorio.h"
#include "ac_payload_extension.H"


// If you don't want to measure dir access, modify measures_dir to 0
#define measures 1

long  unsigned int count_dir = 0;
FILE *local_dir_file;
FILE *global_dir_file;

using user::tlm_diretorio;
using tlm::tlm_command;
using tlm::TLM_READ_COMMAND;
using tlm::TLM_WRITE_COMMAND;

tlm_diretorio::tlm_diretorio(sc_module_name module_name) :
  sc_module( module_name ),
  LOCAL_target_socket("LOCAL_target_socket")
{
    
    LOCAL_target_socket.register_b_transport(this, &tlm_diretorio::b_transport);
    //target_export( *this );
    value = 0;
	dir = new Diretorio();
	//dir->start(2,64);
	//local_dir_file = fopen ("dir_acess.txt","a");
    
}

/// Destructor
tlm_diretorio::~tlm_diretorio() {
  
 if (measures) 
  {
     global_dir_file = fopen (GLOBAL_FILE_MEASURES_NAME,"a");
     local_dir_file = fopen (LOCAL_FILE_MEASURES_NAME,"a");
     fprintf(global_dir_file, "\nDir Access:\t%ld", count_dir);
     fprintf(local_dir_file, "\nDir Access:\t%ld", count_dir);
     printf("\nDir Access:\t%ld", count_dir);
     fclose (local_dir_file);
     fclose (global_dir_file);
  }
 delete dir;
}

void tlm_diretorio::b_transport( ac_tlm2_payload &payload, sc_core::sc_time &time_info ) {


   
   ac_payload_extension *payloadExt;
   time_info = time_info + sc_core::sc_time(1,SC_NS);
   int nCache=-1; 
   bool validation=false;
   payload.get_extension(payloadExt);

   if(payloadExt != 0)
   {

   		count_dir++;
	    nCache = payloadExt->getnumberCache(); //id do processador
		uint32_t address = payloadExt->getAddress(); //endereco do dado armazenado
		int cacheIndex = payloadExt->getCacheIndex(); //indice do vetor da cache
		int regra = payloadExt->getRule(); //comando requisitado

		if(regra == 0){
			int nWay = payloadExt->getNWay();
			int index_size = payloadExt->getIndex_size();
			dir->start(nWay, index_size);
			printf("\ntlm_diretorio::b_transport()->rule 0 - starting directory");
		}

		if(regra == 1) //Cache leu dado da memoria, atualizar valor no diretorio
		{
			printf("\ntlm_diretorio::b_transport()->rule 1");
			dir->validate(nCache, address, cacheIndex);
		}
		if(regra==2) //aconteceu escrita na memoria, invalidar todos os outros processadores diferentes de nCache
		{
			printf("\ntlm_diretorio::b_transport()->rule 2");
			dir->unvalidate(nCache, address, cacheIndex);
		}
		if(regra == 3) //Verificar se dado da cache esta Valido
		{
			
			validation = dir->checkValidation(nCache, address, cacheIndex);
			payloadExt->setValidation(validation);
			printf("\ntlm_diretorio::b_transport()->rule 3 validation = %d", validation);

		}
	}
	else 
	{
		printf ("\nERROR: Empty payload extension in tlm_diretorio::b_transport().");
		exit(0);
	}

   /*
   tlm_command command = payload.get_command();
   uint32_t addr = (uint32_t) payload.get_address();
  
   unsigned char* data_pointer = payload.get_data_ptr();
  
   unsigned int len = payload.get_data_length();
   
   unsigned int resp;
   */

   payload.set_response_status(tlm::TLM_OK_RESPONSE);    
   

   return;
    
  }





