/********************************************************************************
	MPSoCBench Benchmark Suite
	Authors: Liana Duenha
	Supervisor: Rodolfo Azevedo
	Date: July-2012
	www.archc.org/benchs/mpsocbench

	Computer Systems Laboratory (LSC)
	IC-UNICAMP
	http://www.lsc.ic.unicamp.br/


	This source code is part of the MPSoCBench Benchmark Suite, which is a free
	source-code benchmark for evaluation of Electronic Systemc Level designs.
	This benchmark is distributed with hope that it will be useful, but
	without any warranty.

*********************************************************************************/

#include "tlm_node.h"

using user::tlm_node;
using user::tlm_payload_extension;
using user::tlm_empty_master_node;
using user::tlm_empty_slave_node;
using tlm::tlm_extension_base;



unsigned int tlm_node::numberOfNodes = 0;


tlm_node::tlm_node() :
  module_name("noc_node"),
  sc_module((sc_module_name) module_name),
  N_port("N_port",1U), 
  S_port("S_port",4U), 
  W_port("W_port",8U), 
  E_port("E_port",16U), 
  LOCAL_port("LOCAL_port",32U)
  {

	LOCAL_export( *this );
	setId(numberOfNodes++);
	setStatus(OFF);

  }

tlm_node::tlm_node( sc_module_name module_name) :
  sc_module( module_name ),
  N_port("N_port",1U), 
  S_port("S_port",4U), 
  W_port("W_port",8U), 
  E_port("E_port",16U), 
  LOCAL_port("LOCAL_port",32U)
  {

	LOCAL_export( *this );
	setId(numberOfNodes++);
	setStatus(OFF);

  }

tlm_node::~tlm_node()
{
 
}


void tlm_node::b_transport(ac_tlm2_payload& payload, sc_core::sc_time& time_info)
{


	time_info = time_info + sc_core::sc_time(TIME_NODE+TIME_NODE,SC_NS);
	
	//wait(1,SC_NS);


    user::tlm_payload_extension *ex;
  	tlm::tlm_extension_base* base;
	base = payload.get_extension(1);

   	ex = reinterpret_cast<user::tlm_payload_extension*>(base);


	//user::tlm_payload_extension *ex;
	//payload.get_extension(ex);

	if (getStatus() == OFF)
	{
		printf("ERROR--> i'm in transport method in a noc node with status OFF ");
		exit(1);
	}

	if (getX() == ex->getTargetX())
	{
		if (getY() == ex->getTargetY())
		{
			if (NOC_DEBUG) { printf("\nTransporting to the final node using LOCAL port. delay = +");
			time_info.print() ; }
			LOCAL_port->b_transport(payload,time_info);
		}
		else if ( getY() > ex->getTargetY() )
		{
			
			if (NOC_DEBUG) { printf("\nTransporting using west port. delay = +");		
			time_info.print(); }
			W_port->b_transport(payload,time_info);
			
		}
		else  {
			if (NOC_DEBUG)	{ printf("\nTransporting using east port. delay = +");
			time_info.print(); }
			E_port->b_transport(payload,time_info);
		}
	}
	else if (getX() > ex->getTargetX())
	{
		if (NOC_DEBUG) { printf ("\nTransporting using north port. delay = +"); 
		time_info.print(); }
		N_port->b_transport(payload,time_info);
	}
	else {
		if (NOC_DEBUG) { printf("\nTransporting using south port. delay = +");
		time_info.print() ; }
		S_port->b_transport(payload,time_info);
	}

	if (NOC_DEBUG)
    	{	
	    printf("\ntlm_node::b_transport returning");
   	}

	//wait(1,SC_NS);

}




tlm_empty_slave_node::tlm_empty_slave_node() :
  module_name("empty_slave_node"),
  sc_module( (sc_module_name) module_name )
  {
	LOCAL_export( *this );
		
  }

tlm_empty_master_node::tlm_empty_master_node() :
  module_name("empty_master_node"),
  sc_module( (sc_module_name) module_name ),
  LOCAL_port("LOCAL_port",1U)  
  {
		
  }

 tlm_empty_slave_node::tlm_empty_slave_node(sc_module_name mod_name) :
  sc_module (mod_name)
  {
	LOCAL_export( *this );
		
  }


 tlm_empty_master_node::tlm_empty_master_node(sc_module_name mod_name) :
  sc_module( mod_name ),
  LOCAL_port("LOCAL_port",1U)  
  {
		
  }


