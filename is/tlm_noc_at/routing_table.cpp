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
 * @file      routing_table.cpp
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

#include "routing_table.h"

using user::routing_table;

uint64_t routing_table::baseMemoryAddress = BASE_MEMORY_ADDRESS;

/// Constructor

void routing_table::newEntry(int i, int j, uint64_t a) {
  m[numberOfActiveLines][0] = a;
  m[numberOfActiveLines][1] = i;
  m[numberOfActiveLines][2] = j;

  numberOfActiveLines++;
}

void routing_table::newEntry(int i, int j) {

  baseMemoryAddress = baseMemoryAddress + DELTA;
  m[numberOfActiveLines][0] = baseMemoryAddress;
  m[numberOfActiveLines][1] = i;
  m[numberOfActiveLines][2] = j;
  numberOfActiveLines++;
}

routing_table::routing_table() {

  numberOfActiveLines = 0;
  for (int i = 0; i < MAX_LINES; i++)
    for (int j = 0; j < MAX_COLUMNS; j++)
      m[i][j] = 0;
}

void routing_table::print() {
  printf("\nNumber of Active Lines in the table of routs: %d \n",
         numberOfActiveLines);
  for (int i = 0; i < numberOfActiveLines; i++) {
    printf("m[%d]: %lu %lu %lu\n ", i, (long unsigned int)m[i][0],
           (long unsigned int)m[i][1], (long unsigned int)m[i][2]);
  }
}

void routing_table::copyFrom(routing_table &other) {
  this->setNumberOfActiveLines(other.getNumberOfActiveLines());

  for (int i = 0; i < getNumberOfActiveLines(); i++)
    for (int j = 0; j < MAX_COLUMNS; j++)
      m[i][j] = other.m[i][j];
}

void routing_table::returnsTargetPosition(uint64_t addr, int &targetPosX,
                                          int &targetPosY) {

  uint64_t baseAddress = 0;

  int i;

  for (i = 0; i < getNumberOfActiveLines(); i++) {
    if ((addr >= baseAddress) && (addr < m[i][0])) {
      baseAddress = m[i][0];
      targetPosX = m[i][1];
      targetPosY = m[i][2];
      break;
    }
  }

  if (i == getNumberOfActiveLines()) {
    printf("\nROUTING TABLE ERROR: Address outside the range->%lu  i-->%d\n",
           (long unsigned int)addr, i);
    exit(1);
  }
}
