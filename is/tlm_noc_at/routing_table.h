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
 * @file      routing_table.h
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
 * @brief     Defines the routing table for the TLM 2.0 NoC
 *
 *
 *******************************************************************************/


#ifndef ROUTING_TABLE_H
#define ROUTING_TABLE_H

//////////////////////////////////////////////////////////////////////////////

#include <systemc>

//////////////////////////////////////////////////////////////////////////////

#include "../../defines.h"
//////////////////////////////////////////////////////////////////////////////

/// Namespace to isolate router from ArchC
namespace user
{

/// A TLM memory
class routing_table
{
public:

  routing_table();
  ~routing_table() { }

  static uint64_t baseMemoryAddress;
  
  void newEntry (int, int, uint64_t);
  void newEntry (int, int);
  void print();
   
  int getNumberOfActiveLines () { return numberOfActiveLines; }
  void setNumberOfActiveLines (int a) { numberOfActiveLines = a; }

  uint64_t m[MAX_LINES][MAX_COLUMNS];

  void copyFrom (routing_table &other);
  void returnsTargetPosition(uint64_t addr,int &targetPosX, int &targetPosY);

private:  
  int numberOfActiveLines;
};

};

#endif 
