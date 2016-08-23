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
//////////////////////////////////////////////////////////////////////////////

#ifndef tlm_lock_H_
#define tlm_lock_H_

//////////////////////////////////////////////////////////////////////////////

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc>
// ArchC includes
#include "ac_tlm_protocol.H"

#include "tlm.h"

#include "../../defines.h"

#include "tlm_utils/simple_target_socket.h"

//////////////////////////////////////////////////////////////////////////////

// using statements

// using tlm::tlm_blocking_transport_if;
// using tlm::tlm_dmi;

//////////////////////////////////////////////////////////////////////////////

/// Namespace to isolate memory from ArchC
namespace user {

/// A TLM memory
class tlm_lock : public sc_module {

public:
  tlm_utils::simple_target_socket<tlm_lock> LOCAL_target_socket;

  uint32_t readm(uint32_t &);

  /**
   * Implementation of TLM transport method that
   * handle packets of the protocol doing apropriate actions.
   * This method must be implemented (required by SystemC TLM).
   * @param request is a received request packet
   * @return A response packet to be send
  */
  void b_transport(ac_tlm2_payload &, sc_core::sc_time &);

  /**
   * Default constructor.
   *
   * @param k Memory size in kilowords.
   *
   */

  tlm_lock(sc_module_name module_name);
  /**
   * Default destructor.
   */
  ~tlm_lock();

private:
  uint32_t value;
};
};

#endif // tlm_lock_H_
