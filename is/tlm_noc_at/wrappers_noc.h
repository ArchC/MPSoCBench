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
/******************************************************************************
 * @file      tlm_node.h
 * @author    Liana Duenha
 *
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 * 
 * @date      01, Feb, 2013
 * @brief     Defines a set of wrappers for a TLM 2.0 NoC
 *
 *
 *******************************************************************************/


#ifndef TLM_WRAPPER_NOC_H_
#define TLM_WRAPPER_NOC_H_

//////////////////////////////////////////////////////////////////////////////
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc>
#include <tlm.h>
#include "ac_tlm_protocol.H"
#include "tlm_payload_extension.h"
//#include "tlm_payload_dir_extension.H"
#include "routing_table.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "local_dfs.h"
#include "../../defines.h"

//////////////////////////////////////////////////////////////////////////////

//using user::tlm_payload_extension;
using user::routing_table;
using namespace sc_core;

#ifdef POWER_SIM
using user::local_dfs;
#endif


namespace user
{


class wrapper_master_slave_to_noc:  public sc_module		 
{
public:
 
  tlm_utils::simple_initiator_socket<wrapper_master_slave_to_noc> LOCAL_init_socket;
  tlm_utils::simple_target_socket<wrapper_master_slave_to_noc> LOCAL_target_socket;

  tlm_utils::simple_initiator_socket< wrapper_master_slave_to_noc> NODE_init_socket;
  tlm_utils::simple_target_socket< wrapper_master_slave_to_noc> NODE_target_socket;
  
  const char *module_name;

  routing_table tableOfRouts;

  SC_HAS_PROCESS( wrapper_master_slave_to_noc );
  wrapper_master_slave_to_noc(); 
  wrapper_master_slave_to_noc(sc_module_name module_name);

  ~wrapper_master_slave_to_noc() { 

      #ifdef POWER_SIM
      if (dfs!=NULL) delete dfs;
      #endif

  }

  inline void setStatus(int st){
	this->status = st;
  }

  inline int getStatus() {
	return this->status;
  }
  
  inline int getX() {
  	return this->posX;
  }


  inline int getY() {
  	return this->posY;
  }

 
  inline void setX(int a) {
  	this->posX = a;
  }
  inline void setY(int b) {
  	this->posY = b;
  }


  inline long int getNumberOfHops() {
  	return this->numberOfHops;
  }
  inline long int getNumberOfPackages() {
  	return this->numberOfPackages;
  }
 
  void b_transport(ac_tlm2_payload &, sc_core::sc_time &) {}
  tlm::tlm_sync_enum  nb_transport_fw(ac_tlm2_payload &, tlm::tlm_phase &, sc_core::sc_time &);
  tlm::tlm_sync_enum  nb_transport_bw(ac_tlm2_payload &, tlm::tlm_phase &, sc_core::sc_time &);

  #ifdef POWER_SIM
  local_dfs *dfs;
  void initDFS (PROCESSOR_NAME* proc);
  #endif

private:

  unsigned int posX;
  unsigned int posY;

  unsigned int status;

  /* NUMBER OF HOPS */
  long int numberOfHops;
  long int numberOfPackages;
  /* NUMBER OF HOPS */



  
  
  
};


};
#endif
