

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
       source-code benchmark for evaluation of Electronic Systemc Level designs.
       This benchmark is distributed with hope that it will be useful, but
       without any warranty.

*********************************************************************************/
/**
 * @file      tlm_intr_ctrl.h
 * @author    Liana Duenha
 *
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @version   0.1
 * @date      Sun, 20 Oct 2014
 * @brief     Defines a TLM 2.0 Interruption Controller


 * @attention Copyright (C) 2002-2012 --- The ArchC Team
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

#include <stdint.h>
#include "tlm_intr_ctrl.h"
#include <tlm.h>

#define measures 1

using user::tlm_intr_ctrl;
using tlm::tlm_command;
using tlm::TLM_READ_COMMAND;
using tlm::TLM_WRITE_COMMAND;

tlm_intr_ctrl::tlm_intr_ctrl(sc_module_name module_name, int n)
    : sc_module(module_name), target_export("target_export") {
  target_export(*this);

  workers = n;
  CPU_port = new sc_port<ac_tlm2_blocking_transport_if>[workers];
}

tlm_intr_ctrl::~tlm_intr_ctrl() {}

void tlm_intr_ctrl::b_transport(ac_tlm2_payload &payload,
                                sc_core::sc_time &time_info) {

  // forward and backward paths
  uint32_t addr = (uint32_t)payload.get_address();

  tlm_command command = payload.get_command();

  unsigned int procId = payload.get_streaming_width();

  unsigned char *d = payload.get_data_ptr();

  uint32_t *T = reinterpret_cast<uint32_t *>(d);
  int intr;

  switch (command) {
  case TLM_READ_COMMAND:

    break;

  case TLM_WRITE_COMMAND:

    if (INTR_CTRL_DEBUG) {
      printf("\nTLM_INTR_CTRL: received a WRITE request from processor %d",
             procId);
    }

    intr = (int)T[0];

#ifdef AC_GUEST_BIG_ENDIAN
    intr = be32toh(intr);
#endif

    if (intr == ON)
      turnOnProcessors(procId);

    else if (intr == OFF)
      turnOffProcessors(procId);

    else
      printf("\nTLM_INTR_CTRL: Interruption code %d not recognized...Ignored.",
             intr);

    payload.set_response_status(tlm::TLM_OK_RESPONSE);
  default:
    break;
  }

  if (INTR_CTRL_DEBUG)
    printf("\ntlm_intr_ctrl b_transport returning");
}

// MODE CONTROL: OFF / ON
// procId is the processor that send the interruption to turning on all
// processors
void tlm_intr_ctrl::turnOnProcessors(int procId) {

  if (INTR_CTRL_DEBUG)
    printf("\nTLM_INTR_CTRL: turninng on all processors \n");
  for (int i = 0; i < workers; i++) {
    if (i != procId)
      send(i, ON);
  }
}
// procId is the processor that send the interruption to turning off all
// processors (except itself)
void tlm_intr_ctrl::turnOffProcessors(int procId) {

  if (INTR_CTRL_DEBUG)
    printf("\nTLM_INTR_CTRL: turninng off all processors \n");
  for (int i = 0; i < workers; i++) {
    if (i != procId)
      send(i, OFF);
  }
}

void tlm_intr_ctrl::send(int id, int intr) {

  if (INTR_CTRL_DEBUG)
    printf(
        "\nTLM_INTR_CTRL: sending to processor %d an interruption with code %d",
        id, intr);
  sc_core::sc_time time_info = sc_time_stamp();
  unsigned char p[4];

  uint32_t *T = reinterpret_cast<uint32_t *>(p);
  T[0] = intr;

  payload.set_command(tlm::TLM_WRITE_COMMAND);
  payload.set_data_ptr(p);

  CPU_port[id]->b_transport(payload, time_info);
}
