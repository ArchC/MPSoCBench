/*******************************************************
 * This is the part of the MPSoCBench benchmark suite  *
 * If you want more information on MPSoCBench or ArchC,*
 * please visit:                                       *
 * http://www.archc.org/benchs/mpsocbench , or         *
 * http://www.archc.org                                *
 * Computer Systems Laboratory (LSC)                   *
 * IC-UNICAMP                                          *
 * http://www.lsc.ic.unicamp.br                        *
 ******************************************************/



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
#include "ac_tlm2_nb_port.H"

#include "tlm_node.h"
#include "routing_table.h"
#include "wrappers_noc.h"
#include "../../defines.h"

//////////////////////////////////////////////////////////////////////////////

using tlm::tlm_blocking_transport_if;
using user::tlm_node;
using user::tlm_master_node;
using user::tlm_slave_node;
using user::wrapper_master_slave_to_noc;

//////////////////////////////////////////////////////////////////////////////


namespace user
{

class tlm_noc:
  public sc_module   
{
public:

  tlm_noc( sc_module_name module_name, int, int, int, int);
  ~tlm_noc();
 
  void create();
  void init();
  int getNodeIndex(int,int);
  void bindingInternalPorts();
  void preparingRoutingTable();

  inline void setNumberOfMasters(unsigned  int); 
  inline void setNumberOfSlaves(unsigned int ); 
  inline void setNumberOfMasterEmptyNodes(unsigned int); 
  inline void setNumberOfSlaveEmptyNodes(unsigned int ); 

  inline void setNumberOfPeripherals(unsigned int); 
  inline void setNumberOfInactiveNodes(unsigned int); 
  inline void setNumberOfLines(unsigned int); 
  inline void setNumberOfColumns(unsigned int); 
  inline void setNumberOfNodes(unsigned int);
  inline void setNumberOfWrappers(unsigned int);

  void print();

  inline unsigned int getNumberOfMasters();
  inline unsigned int getNumberOfSlaves();
  inline unsigned int getNumberOfMasterEmptyNodes();
  inline unsigned int getNumberOfSlaveEmptyNodes();
  inline unsigned int getNumberOfWrappers();
 
  unsigned int getNumberOfNodes();
  inline unsigned int getNumberOfPeripherals(); 
  inline unsigned int getNumberOfInactiveNodes();
  
  unsigned int getNumberOfLines();
  unsigned int getNumberOfColumns();

  tlm_node **mesh;
  routing_table tableOfRouts; 
  tlm_slave_node  *slaveEmptyNodes;
  tlm_master_node *masterEmptyNodes;
 
  wrapper_master_slave_to_noc *wrapper;
 

private:


  unsigned int numberOfPeripherals;
  unsigned int numberOfSlaves;
  unsigned int numberOfMasters;
  unsigned int numberOfInactiveNodes;
  unsigned int numberOfLines;
  unsigned int numberOfColumns;
  unsigned int numberOfNodes;
  unsigned int numberOfMasterEmptyNodes;
  unsigned int numberOfSlaveEmptyNodes;
  unsigned int numberOfWrappers;

};

};

#endif 
