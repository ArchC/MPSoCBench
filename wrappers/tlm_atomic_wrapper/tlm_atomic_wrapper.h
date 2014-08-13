/********************************************************
 * This is the part of the MPSoCBench benchmark suite   *
 * If you want more information on MPSoCBench or ArchC, *
 * please visit:                                        *
 * http://archc.org/benchs/mpsocbench/ , or             *
 * http://www.archc.org                                 *
 * Computer Systems Laboratory (LSC)                    *
 * IC-UNICAMP                                           *
 * http://www.lsc.ic.unicamp.br                         *
 *******************************************************/


/**
 * @file      wrapper_exclusive.h
 * @author    Liana (TLM 2.0)
 *
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 * 
 * @date      Wed, 25 March 2013 
 * @brief     Defines a wrapper useful to connect the communication device to shared resources 
 */

//////////////////////////////////////////////////////////////////////////////

#ifndef WRAPPER_EXCLUSIVE_H_
#define WRAPPER_EXCLUSIVE_H_


#include <systemc>
#include "ac_tlm_protocol.H"
#include "ac_tlm2_port.H"

#include "../../defines.h"


using tlm::tlm_blocking_transport_if;

namespace user
{

/// A TLM memory
class tlm_atomic_wrapper:
  public sc_module,
  public ac_tlm2_blocking_transport_if 
{
public:
  ac_tlm2_port LOCAL_port;  
  sc_export< ac_tlm2_blocking_transport_if  > target_export;
  pthread_mutex_t mymutex;

  tlm_atomic_wrapper(sc_module_name module_name);
  ~tlm_atomic_wrapper();
   void b_transport(ac_tlm2_payload &, sc_core::sc_time &);


};

};

#endif 
