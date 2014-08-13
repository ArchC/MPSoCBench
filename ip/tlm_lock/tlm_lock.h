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



/**
 * @file      tlm_lock.h
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
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
 *   Defines a lock device, useful for concurrency management.
 */

#ifndef tlm_lock_H_
#define tlm_lock_H_

#include <systemc>
#include "ac_tlm_protocol.H"
#include "ac_tlm2_port.H"
#include "../../defines.h"

using tlm::tlm_blocking_transport_if;

/// Namespace to isolate memory from ArchC
namespace user
{

class tlm_lock :
  public sc_module,
  public ac_tlm2_blocking_transport_if // Using ArchC TLM protocol
{
public:

   sc_export< ac_tlm2_blocking_transport_if > target_export;
   
   uint32_t readm (uint32_t & );
   void b_transport(ac_tlm2_payload &, sc_core::sc_time &);
   tlm_lock( sc_module_name module_name);
   ~tlm_lock();

private:
   uint32_t value;

};

};

#endif //tlm_lock_H_
