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


#ifndef TLM_WRAPPER_NOC_H
#define TLM_WRAPPER_NOC_H

#include <systemc>
#include <tlm.h>
#include "ac_tlm_protocol.H"
#include "tlm_payload_extension.h"
#include "routing_table.h"
#include "ac_tlm2_port.H" 
//#include "local_dfs.h"
#include "../../defines.h"

using tlm::tlm_blocking_transport_if;
using user::routing_table;

using namespace sc_core;

namespace user
{


class wrapper_noc:
  public sc_module,
  public ac_tlm2_blocking_transport_if 
{
public:

  wrapper_noc();
  wrapper_noc(sc_module_name module_name);
  ~wrapper_noc() {  } 
  void b_transport(ac_tlm2_payload& payload, sc_core::sc_time& time_info);

   inline void setStatus(int st){
  this->status = st;
  }

  inline int getStatus() {
  return this->status;
  }
  
  inline int getX() {
    return this->posX;
  }
  inline int getY() {
    return this->posY;
  }

  inline void setX(int a) {
    this->posX = a;
  }
  inline void setY(int b) {
    this->posY = b;
  }
  
  
  const char *module_name;
  ac_tlm2_port LOCAL_port;
  ac_tlm2_port NODE_port;
  sc_export <ac_tlm2_blocking_transport_if> LOCAL_export;

  routing_table tableOfRouts;


  
 private:
   
   int posX;
   int posY;
   int status;
    
 
};

};

#endif 
