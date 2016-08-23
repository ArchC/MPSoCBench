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

#ifndef TLM_NODE_H_
#define TLM_NODE_H_

//////////////////////////////////////////////////////////////////////////////

#include <systemc>
#include <tlm.h>
#include "ac_tlm_protocol.H"
#include "tlm_payload_extension.h"
#include "ac_tlm2_port.H"
//#include "routing_table.h"
#include "../../defines.h"

//////////////////////////////////////////////////////////////////////////////

using tlm::tlm_blocking_transport_if;
// using user::routing_table;

//////////////////////////////////////////////////////////////////////////////

namespace user {

class tlm_node : public sc_module, public ac_tlm2_blocking_transport_if {
public:
  ac_tlm2_port N_port;
  ac_tlm2_port S_port;
  ac_tlm2_port W_port;
  ac_tlm2_port E_port;

  ac_tlm2_port LOCAL_port;
  sc_export<ac_tlm2_blocking_transport_if> LOCAL_export;

  const char *module_name;

  static unsigned int numberOfNodes;

  // routing_table tableOfRouts;

  tlm_node();
  tlm_node(sc_module_name module_name);
  ~tlm_node();

  inline void setStatus(int st) { this->status = st; }
  inline int getId() { return this->id; }
  inline int getStatus() { return this->status; }

  inline int getX() { return this->posX; }
  inline int getY() { return this->posY; }

  inline void setId(int a) { this->id = a; }
  inline void setX(int a) { this->posX = a; }
  inline void setY(int b) { this->posY = b; }

  void b_transport(ac_tlm2_payload &, sc_core::sc_time &);

private:
  unsigned int id;
  unsigned int posX;
  unsigned int posY;

  unsigned int status;
};

class tlm_empty_slave_node : public sc_module,
                             public ac_tlm2_blocking_transport_if {
public:
  sc_export<ac_tlm2_blocking_transport_if> LOCAL_export;

  const char *module_name;

  tlm_empty_slave_node();
  tlm_empty_slave_node(sc_module_name mod_name);

  ~tlm_empty_slave_node() {}
  void b_transport(ac_tlm2_payload &payload, sc_core::sc_time &time_info) {}
};

class tlm_empty_master_node : public sc_module,
                              public ac_tlm2_blocking_transport_if {
public:
  ac_tlm2_port LOCAL_port;

  const char *module_name;

  tlm_empty_master_node();
  tlm_empty_master_node(sc_module_name mod_name);
  ~tlm_empty_master_node() {}
  void b_transport(ac_tlm2_payload &payload, sc_core::sc_time &time_info) {}
};
};

#endif
