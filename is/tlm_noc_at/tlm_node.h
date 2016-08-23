/********************************************************************************
        MPSoCBench Benchmark Suite
        Authors: Liana Duenha
        Supervisor: Rodolfo Azevedo
        Date: July-2012
        www.archc.org/benchs/mpsocbench

        Computer Systems Laboratory (LSC)
        IC-UNICAMP
        http://www.lsc.ic.unicamp.br/


        This source code is part of the MPSoCBench Benchmark Suite, which is a
free
        source-code benchmark for evaluation of Electronic Systemc Level
designs.
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
 * @brief     Defines a node of TLM 2.0 NoC
 *
 *
 *******************************************************************************/

#ifndef TLM_NODE_H_
#define TLM_NODE_H_

//////////////////////////////////////////////////////////////////////////////
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc>
#include <tlm.h>
#include "ac_tlm_protocol.H"
#include "tlm_payload_extension.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "../../defines.h"
#include "powersc_power_stats.H"

using tlm::tlm_dmi;

using namespace sc_core;

//////////////////////////////////////////////////////////////////////////////
namespace user {

class packageType {
public:
  packageType(ac_tlm2_payload &p, tlm::tlm_phase &ph, sc_core::sc_time &t);
  ~packageType();
  ac_tlm2_payload payload;
  sc_core::sc_time time;
  tlm::tlm_phase phase;
  packageType *next;
  packageType *prev;
};

class tlm_node : public sc_module {
public:
  tlm_utils::simple_initiator_socket<tlm_node> LOCAL_init_socket;
  tlm_utils::simple_initiator_socket<tlm_node> N_init_socket;
  tlm_utils::simple_initiator_socket<tlm_node> S_init_socket;
  tlm_utils::simple_initiator_socket<tlm_node> W_init_socket;
  tlm_utils::simple_initiator_socket<tlm_node> E_init_socket;

  tlm_utils::simple_target_socket<tlm_node> LOCAL_target_socket;
  tlm_utils::simple_target_socket<tlm_node> N_target_socket;
  tlm_utils::simple_target_socket<tlm_node> S_target_socket;
  tlm_utils::simple_target_socket<tlm_node> W_target_socket;
  tlm_utils::simple_target_socket<tlm_node> E_target_socket;

  const char *module_name;

  static unsigned int numberOfNodes;

  sc_event wake_up;

#ifdef POWER_SIM
  is_power_stats *ps;
#endif

  // pthread_mutex_t listPackageMutex;

  SC_HAS_PROCESS(tlm_node);

  tlm_node();

  tlm_node(sc_module_name module_name);

  ~tlm_node();

  inline void setStatus(int st) { this->status = st; }

  inline int getStatus() { return this->status; }

  inline int getX() { return this->posX; }
  inline int getY() { return this->posY; }

  inline void setX(int a) { this->posX = a; }
  inline void setY(int b) { this->posY = b; }

  void b_transport(ac_tlm2_payload &, sc_core::sc_time &);
  tlm::tlm_sync_enum nb_transport_fw(ac_tlm2_payload &, tlm::tlm_phase &,
                                     sc_core::sc_time &);
  tlm::tlm_sync_enum nb_transport_bw(ac_tlm2_payload &, tlm::tlm_phase &,
                                     sc_core::sc_time &);

  void thread_node();

  void addToBuffer(ac_tlm2_payload &, tlm::tlm_phase &, sc_core::sc_time &);
  void removeFromBuffer(ac_tlm2_payload &, tlm::tlm_phase &,
                        sc_core::sc_time &);
  int getNumberOfPackagesInBuffer() { return numberOfPackagesInBuffer; }

  int counterS; // south
  int counterN; // north
  int counterW; // west
  int counterE; // east
  int counterL; // local

private:
  unsigned int posX;
  unsigned int posY;

  unsigned int status;

  /*********/
  packageType *beginListPackage;
  packageType *endListPackage;
  int numberOfPackagesInBuffer;
  /*********/
};

class tlm_slave_node : public sc_module {
public:
  tlm_utils::simple_target_socket<tlm_slave_node> LOCAL_slave_target_socket;

  const char *module_name;

  tlm_slave_node(sc_module_name mod_name);
  tlm_slave_node();

  ~tlm_slave_node() {}

  inline void setStatus(int st) { this->status = st; }
  inline int getStatus() { return this->status; }

  void b_transport(ac_tlm2_payload &, sc_core::sc_time &) {}
  tlm::tlm_sync_enum nb_transport_fw(ac_tlm2_payload &, tlm::tlm_phase &,
                                     sc_core::sc_time &) {}

private:
  unsigned int status;
};

class tlm_master_node : public sc_module {
public:
  tlm_utils::simple_initiator_socket<tlm_master_node> LOCAL_master_init_socket;

  const char *module_name;
  int status;

  tlm_master_node(sc_module_name mod_name);
  tlm_master_node();

  ~tlm_master_node() {}

  void b_transport(ac_tlm2_payload &, sc_core::sc_time &) {}
  tlm::tlm_sync_enum nb_transport_bw(ac_tlm2_payload &, tlm::tlm_phase &,
                                     sc_core::sc_time &) {}

  void setStatus(int st) { this->status = st; }
  int getStatus() { return this->status; }
};
};
#endif
