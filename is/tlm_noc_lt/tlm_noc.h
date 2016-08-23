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
 * @file      tlm_noc.h
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
 * @brief     Defines a TLM 2.0 NOC
 *
 *******************************************************************************/

#ifndef TLM_NOC_H_
#define TLM_NOC_H_
#define SC_INCLUDE_DYNAMIC_PROCESSES
//////////////////////////////////////////////////////////////////////////////

#include <systemc>
#include "ac_tlm_protocol.H"
#include "tlm_node.h"
#include "routing_table.h"
#include "wrappers_noc.h"
#include "../../defines.h"

//////////////////////////////////////////////////////////////////////////////

using tlm::tlm_blocking_transport_if;
using user::tlm_node;

//////////////////////////////////////////////////////////////////////////////

namespace user {

class tlm_noc : public sc_module {
public:
  tlm_noc(sc_module_name module_name, int, int, int, int);
  // tlm_noc(int, int, int, int);
  ~tlm_noc();

  void create();
  void init();

  int getNodeIndex(int, int);
  void bindingInternalPorts();
  void bindingEmptySlave(int);
  void preparingRoutingTable();

  void setNumberOfMasters(int);
  void setNumberOfSlaves(int);
  void setNumberOfPeripherals(int);
  void setNumberOfInactiveNodes(int);
  void setNumberOfLines(int);
  void setNumberOfColumns(int);
  void setNumberOfNodes(int);
  void setNumberOfWrappers(int);
  int getNumberOfMasters();
  int getNumberOfSlaves();
  int getNumberOfNodes();
  int getNumberOfWrappers();
  int getNumberOfPeripherals();
  int getNumberOfInactiveNodes();
  int getNumberOfLines();
  int getNumberOfColumns();
  void print();

  tlm_node **mesh;
  routing_table tableOfRouts;
  wrapper_noc *wrapper;

private:
  static int numberOfMasterEmptyNodes;
  tlm_empty_slave_node *edgesEmptyNode;

  int numberOfPeripherals;
  int numberOfSlaves;
  int numberOfMasters;
  int numberOfInactiveNodes;
  int numberOfLines;
  int numberOfColumns;
  int numberOfNodes;
  int numberOfWrappers;
};
};

#endif
