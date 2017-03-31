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

/**
 * @file      tlm_router.h
 * @author    Rodolfo Azevedo
 * @author    Liana (TLM 2.0)
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
 * @brief     Defines a ac_tlm router.
 *
 * @date      Wed, 22 Oct 2012
 *            Modified by Liana Duenha
 * @brief     Defines a TLM 2.0 router using blocking transport method.

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

#ifndef TLM_ROUTER_H_
#define TLM_ROUTER_H_

#include <systemc>
#include "ac_tlm_protocol.H"
#include "ac_tlm2_port.H"
#include "../../defines.h"

using tlm::tlm_blocking_transport_if;

namespace user {

class tlm_router : public sc_module, public ac_tlm2_blocking_transport_if {
public:
  ac_tlm2_port MEM_port;
  ac_tlm2_port LOCK_port;
  ac_tlm2_port INTR_CTRL_port;
  //ac_tlm2_port DIR_port;

#ifdef POWER_SIM
  ac_tlm2_port DFS_port;
#endif

  sc_export<ac_tlm2_blocking_transport_if> target_export;

  tlm_router(sc_module_name module_name);
  ~tlm_router();

  void b_transport(ac_tlm2_payload &, sc_core::sc_time &);
};
};

#endif // TLM_ROUTER_H_
