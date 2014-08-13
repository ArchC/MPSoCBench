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
#ifndef TLM_NOC_H_
#define TLM_NOC_H_

//////////////////////////////////////////////////////////////////////////////

#include <systemc>
#include "ac_tlm_protocol.H"
#include "ac_tlm2_port.H"
#include "tlm_node.h"
#include "routing_table.h"
#include "wrapper_noc.h"
#include "../../defines.h"

//////////////////////////////////////////////////////////////////////////////

using tlm::tlm_blocking_transport_if;
using user::tlm_node;
using user::tlm_empty_master_node;
using user::wrapper_noc;

//////////////////////////////////////////////////////////////////////////////

/// Namespace to isolate router from ArchC
namespace user
{

class tlm_noc:
  public sc_module   
{
public:

  tlm_noc( sc_module_name module_name, int,int,int,int);
  ~tlm_noc();

  void create();
  void init();
  int getNodeIndex(int,int);
  void bindingInternalPorts();
  void bindingEmptySlave(int);
  void preparingRoutingTable();

  inline void setNumberOfMasters(int); 
  inline void setNumberOfSlaves(int ); 
  inline void setNumberOfPeripherals(int); 
  inline void setNumberOfInactiveNodes(int); 
  inline void setNumberOfLines(int); 
  inline void setNumberOfColumns(int); 
  inline void setNumberOfNodes(int);
  inline void setNumberOfWrappers(int);
	
  void print();

  inline int getNumberOfMasters();
  inline int getNumberOfSlaves();
  int getNumberOfNodes();
  int getNumberOfWrappers();
  int getNumberOfPeripherals(); 
  inline int getNumberOfInactiveNodes();
  
  int getNumberOfLines();
  int getNumberOfColumns();

  tlm_node **mesh;
  wrapper_noc *wrapper;

  routing_table tableOfRouts; 
  

private:

  tlm_empty_slave_node edgesEmptyNode;
  static int numberOfMasterEmptyNodes;
  
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
