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
//using tlm::tlm_transport_if;

using tlm::tlm_blocking_transport_if;
using tlm::tlm_dmi;

//////////////////////////////////////////////////////////////////////////////

/// Namespace to isolate memory from ArchC
namespace user
{

/// A TLM memory
class tlm_lock : 
  public sc_module
{

public:
 

 

  tlm_utils::simple_target_socket<tlm_lock> LOCAL_target_socket; 
 
 
  
   uint32_t readm (uint32_t & );
  
  
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

  tlm_lock( sc_module_name module_name);
  /** 
   * Default destructor.
   */
  ~tlm_lock();

private:
  uint32_t value;
  

};

};

#endif //tlm_lock_H_
