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
 * @file      tlm_atomic_wrapper.cpp
 * @author    Liana
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @date      Wed, 25 March 2013
 * @brief     Implements a wrapper useful to connect the communication device to
shared resources

**/

#include "tlm_atomic_wrapper.h"

using user::tlm_atomic_wrapper;

tlm_atomic_wrapper::tlm_atomic_wrapper(sc_module_name module_name)
    : sc_module(module_name), target_export("target_export"),
      LOCAL_port("LOCAL_port", 536870912U) {
  target_export(*this);
  pthread_mutex_init(&mymutex, NULL);
}

tlm_atomic_wrapper::~tlm_atomic_wrapper() {}

void tlm_atomic_wrapper::b_transport(ac_tlm2_payload &payload,
                                     sc_core::sc_time &time_info) {

  uint64_t addr = payload.get_address();

  pthread_mutex_lock(&mymutex);
  if (debugTLM2) {
    printf("\n/**exclusive access starting**/");
    printf("\ntlm_atomic_wrapper is transporting using LOCAL_port");
  }
  LOCAL_port->b_transport(payload, time_info);
  if (debugTLM2) {
    printf("tlm_atomic_wrapper b_transport returning");
    printf("\n/**exclusive access ending**/");
  }
  pthread_mutex_unlock(&mymutex);
}
