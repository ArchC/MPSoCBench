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


#include "wrapper_noc.h"

using user::wrapper_noc;

wrapper_noc::wrapper_noc() :
  module_name("wrapper_node"),
  sc_module((sc_module_name) module_name),
  NODE_port("NODE_port",1U),
  LOCAL_port("LOCAL_port",2U)
  {
        setStatus(OFF);
	LOCAL_export( *this );
  }


wrapper_noc::wrapper_noc(sc_module_name module_name) :
  sc_module( module_name ),
  NODE_port("NODE_port",1U),
  LOCAL_port("LOCAL_port",2U)
  {
        setStatus(OFF);
	LOCAL_export( *this );
  }


void wrapper_noc::b_transport(ac_tlm2_payload& payload, sc_core::sc_time& time_info){
    
    if (NOC_DEBUG) printf("\n\nB_TRANSPORT--> Wrapper %d,%d with status %d is receiving a package",getX(), getY(), getStatus());

    tlm_payload_extension *ex;
    payload.get_extension(ex);

    if (ex == NULL)
    {
	    
	    uint64_t addr = payload.get_address();
	    int targetX, targetY;
	    tableOfRouts.returnsTargetPosition(addr, targetX, targetY);

	    ex = new tlm_payload_extension();
	    ex->setTargetX(targetX);
	    ex->setTargetY(targetY);
	    ex->setInitX(getX());
	    ex->setInitY(getY());
	    ex->setDirection(FORWARD);   // blocking transport has just FORWARD path
  	    payload.set_extension(ex);

	    if (NOC_DEBUG) printf("\nB_TRANSPORT--> Wrapper %d,%d is trasporting package INTO the NOC trought NODE_port", getX(), getY());
	    NODE_port->b_transport(payload,time_info);
 	    
            payload.release_extension(ex);
    }	
    else
    {
	    if (NOC_DEBUG) printf("\nB_TRANSPORT--> Wrapper %d,%d is cleaning payload extension", getX(), getY());

	    
	    if (NOC_DEBUG) printf("\nB_TRANSPORT--> Wrapper %d,%d is trasporting package OUT of NOC trought LOCAL_port", getX(), getY());
	    LOCAL_port->b_transport(payload,time_info);
    }
    
    
    if (NOC_DEBUG)
    {	
	    printf("\n(wrapper_noc::b_transport returning)");
    }
  
      
}
