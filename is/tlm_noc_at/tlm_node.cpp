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


/******************************************************************************
 * @file      tlm_node.cpp
 * @author    Liana Duenha
 *
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 * 
 * @date      01, Feb, 2013
 * @brief     Defines a node of TLM 2.0 NoC
 *
 *
 *******************************************************************************/



#include "tlm_node.h"

/// Namespace to isolate router from ArchC
using user::tlm_node;
using user::tlm_payload_extension;
using user::tlm_slave_node;
using user::tlm_master_node;

using user::packageType;

unsigned int tlm_node::numberOfNodes = 0;


/// Constructor
tlm_node::tlm_node() :
  module_name("noc_node"),
  sc_module((sc_module_name)module_name),
  N_init_socket(), 
  S_init_socket(), 
  W_init_socket(), 
  E_init_socket(), 
  N_target_socket(), 
  S_target_socket(), 
  W_target_socket(), 
  E_target_socket(), 
  LOCAL_init_socket(),
  LOCAL_target_socket()  {

	LOCAL_init_socket.register_nb_transport_bw(this, &tlm_node::nb_transport_bw);
	N_init_socket.register_nb_transport_bw(this, &tlm_node::nb_transport_bw);
	S_init_socket.register_nb_transport_bw(this, &tlm_node::nb_transport_bw);
	W_init_socket.register_nb_transport_bw(this, &tlm_node::nb_transport_bw);
	E_init_socket.register_nb_transport_bw(this, &tlm_node::nb_transport_bw);


	LOCAL_target_socket.register_nb_transport_fw(this, &tlm_node::nb_transport_fw);
	N_target_socket.register_nb_transport_fw(this, &tlm_node::nb_transport_fw);
	S_target_socket.register_nb_transport_fw(this, &tlm_node::nb_transport_fw);
	W_target_socket.register_nb_transport_fw(this, &tlm_node::nb_transport_fw);
	E_target_socket.register_nb_transport_fw(this, &tlm_node::nb_transport_fw);


	setStatus(OFF);

	numberOfPackagesInBuffer = 0;
	SC_THREAD( thread_node );
	sensitive<<wake_up;


    counterS = counterN = counterW = counterE = counterL = 0; 

}

tlm_node::tlm_node( sc_module_name module_name) :
  sc_module( module_name ),
  N_init_socket(), 
  S_init_socket(), 
  W_init_socket(), 
  E_init_socket(), 
  N_target_socket(), 
  S_target_socket(), 
  W_target_socket(), 
  E_target_socket(), 
  LOCAL_init_socket(),
  LOCAL_target_socket()
  {

	LOCAL_init_socket.register_nb_transport_bw(this, &tlm_node::nb_transport_bw);
	N_init_socket.register_nb_transport_bw(this, &tlm_node::nb_transport_bw);
	S_init_socket.register_nb_transport_bw(this, &tlm_node::nb_transport_bw);
	W_init_socket.register_nb_transport_bw(this, &tlm_node::nb_transport_bw);
	E_init_socket.register_nb_transport_bw(this, &tlm_node::nb_transport_bw);

	LOCAL_target_socket.register_nb_transport_fw(this, &tlm_node::nb_transport_fw);
	N_target_socket.register_nb_transport_fw(this, &tlm_node::nb_transport_fw);
	S_target_socket.register_nb_transport_fw(this, &tlm_node::nb_transport_fw);
	W_target_socket.register_nb_transport_fw(this, &tlm_node::nb_transport_fw);
	E_target_socket.register_nb_transport_fw(this, &tlm_node::nb_transport_fw);

	
	setStatus(OFF);
		
	numberOfPackagesInBuffer = 0;
	SC_THREAD(thread_node);


	counterS = counterN = counterW = counterE = counterL = 0; 
	
  }


tlm_node::~tlm_node()
{
 
}



/* TLM transaction phases
--> BEGIN_REQ (Begin Request)
--> END_REQ
--> BEGIN_RESP
--> END_RESP
*/


/* TLM transaction status
--> TLM_ACCEPTED: Transaction, phase and timing arguments unmodified (ignored) on return; 
--> TLM_UPDATED: Transaction, phase and timing arguments updated (used) on return
--> TLM_COMPLETED: Transaction, phase and timing arguments updated (used) on return
*/

tlm::tlm_sync_enum tlm_node::nb_transport_fw(ac_tlm2_payload& payload, tlm::tlm_phase& phase, sc_core::sc_time& time_info)
{
	
	if (NOC_DEBUG) printf("\n\nNB_TRANSPORT_FW-->Node %d,%d with status %d is receiving a package",getX(), getY(), getStatus());
 	
	addToBuffer (payload, phase, time_info);

	if (NOC_DEBUG) printf("\nNotifying a event in FW NODE %d %d", getX(), getY());
	this->wake_up.notify();

	if (NOC_DEBUG) printf("\n\nNB_TRANSPORT_FW --> Node %d,%d is returning with status %d\n",getX(), getY(),tlm::TLM_UPDATED);
	phase = tlm::END_REQ;
	return tlm::TLM_UPDATED;
}


tlm::tlm_sync_enum tlm_node::nb_transport_bw(ac_tlm2_payload& payload, tlm::tlm_phase& phase, sc_core::sc_time& time_info)
{

	if (NOC_DEBUG) printf("\n\nNB_TRANSPORT_BW --> Node %d,%d with status %d is receiving a package",getX(), getY(), getStatus());
 	
	addToBuffer (payload, phase, time_info);

	if (NOC_DEBUG) printf("\nNotifying a event in BW NODE %d %d", getX(), getY());
	this->wake_up.notify();

	if (NOC_DEBUG) printf("\n\nNB_TRANSPORT_BW --> Node %d,%d is returning\n",getX(), getY());
	phase = tlm::END_RESP;
	tlm::tlm_sync_enum status = tlm::TLM_COMPLETED;

	return status;
}


void tlm_node::thread_node ()
{
	
	while(true)
	{
		
		while (getNumberOfPackagesInBuffer() != 0)
		{
			
			ac_tlm2_payload payload;
			sc_core::sc_time time_info;
			tlm::tlm_phase phase;
			tlm::tlm_sync_enum status;

			if (NOC_DEBUG) printf("\nTHREAD NODE--> Node %d,%d",getX(),getY());	
			removeFromBuffer(payload,phase,time_info);
		

			tlm_payload_extension *ex;
  			payload.get_extension(ex);
			

			if (ex == NULL)
			{
				printf("\nTHREAD NODE-->payload with NULL extension");
				exit(1);

			}

			/*NUMBER OF HOPS*/
			ex->incNumberOfHops();
			if (NOC_DEBUG) printf("\nTHREAD_NODE --> Node %d,%d is incrementing the number of hops in the extension (%d)",getX(), getY(), ex->getNumberOfHops()); 	   	
			/*NUMBER OF HOPS*/

			if (getX() == ex->getTargetX())
			{
				if (getY() == ex->getTargetY())
				{
					if (getStatus() == OFF)
					{
						printf("ERROR: Target node %d,%d has status OFF ",getX(), getY());
						exit(1);
					}

					counterL ++;


					if (NOC_DEBUG) printf("\nTHREAD NODE--> The transaction is in the target node %d,%d transporting using LOCAL port.",getX(),getY());	
					if (NOC_DEBUG) printf("\nTHREAD NODE--> Direction:%d.",ex->getDirection());										

					if (ex->getDirection() == FORWARD) {
						
						if (NOC_DEBUG) printf("\nTHREAD NODE--> Node %d,%d is calling nb_transport_fw",getX(), getY());

						
						status = LOCAL_init_socket->nb_transport_fw(payload,phase,time_info);
						if (status != tlm::TLM_COMPLETED)
						{
							printf("\nTHREAD NODE--> Node %d,%d TRANSPORT ERROR (2)", getX(), getY());
							exit(1);
						}
						

				
					}
					else {
						
						
				       		
						if (NOC_DEBUG) printf("\nTHREAD NODE--> Node %d,%d is calling nb_transport_bw",getX(), getY());						
												
						status = LOCAL_target_socket->nb_transport_bw(payload,phase,time_info);
						if (status != tlm::TLM_COMPLETED)
						{
							printf("\nTHREAD NODE--> Node %d,%d TRANSPORT ERROR (2)", getX(), getY());
							exit(1);
						}	

					}
										
				}

				else if ( getY() > ex->getTargetY() )
				{


							
					if (NOC_DEBUG) printf("\nTHREAD NODE--> Node %d,%d transporting using west port.",getX(), getY());	
					if (NOC_DEBUG) printf("\nTHREAD NODE--> Direction:%d.",ex->getDirection());		

					counterW++;

					if (ex->getDirection() == FORWARD) {
						
						status = W_init_socket->nb_transport_fw(payload,phase,time_info); 
						if (status != tlm::TLM_UPDATED)
						{
							printf("\nTHREAD NODE--> Node %d,%d TRANSPORT ERROR (1)", getX(), getY());
							exit(1);
						}	
							
					}
					else {
				
						
						status =  W_target_socket->nb_transport_bw(payload,phase,time_info); 
						if (status != tlm::TLM_COMPLETED)
						{
							printf("\nTHREAD NODE--> Node %d,%d TRANSPORT ERROR (2)", getX(), getY());
							exit(1);
						}	


					}

			
				}
				else  {

					

					if (NOC_DEBUG)	printf("\nTHREAD NODE--> Node %d,%d transporting using east port.",getX(),getY());
					if (NOC_DEBUG) printf("\nTHREAD NODE--> Direction:%d.",ex->getDirection());	

					counterE++;

					if (ex->getDirection() == FORWARD){

						
						status = E_init_socket->nb_transport_fw(payload,phase,time_info);
						if (status != tlm::TLM_UPDATED)
						{
							printf("\nTHREAD NODE--> Node %d,%d TRANSPORT ERROR (1)", getX(), getY());
							exit(1);
						}	

					}
					else {

						
						status = E_target_socket->nb_transport_bw(payload,phase,time_info);
						if (status != tlm::TLM_COMPLETED)
						{
							printf("\nTHREAD NODE--> Node %d,%d TRANSPORT ERROR (2)", getX(), getY());
							exit(1);
						}	


					}

					
				}
			}

			else if (getX() > ex->getTargetX())
			{

				if (NOC_DEBUG) printf ("\nTHREAD NODE--> Node %d,%d transporting using north port.",getX(), getY());
				if (NOC_DEBUG) printf("\nTHREAD NODE--> Direction:%d.",ex->getDirection());	

				counterN++;

				if (ex->getDirection() == FORWARD) {
					
					
					status = N_init_socket->nb_transport_fw(payload,phase,time_info);
					if (status != tlm::TLM_UPDATED)
					{
						printf("\nTHREAD NODE--> Node %d,%d TRANSPORT ERROR (1)", getX(), getY());
						exit(1);
					}	
				}
				else {

					
					status = N_target_socket->nb_transport_bw(payload,phase,time_info);
					if (status != tlm::TLM_COMPLETED)
					{
						printf("\nTHREAD NODE--> Node %d,%d TRANSPORT ERROR (2)", getX(), getY());
						exit(1);
					}	

				}

			}
			else {


				if (NOC_DEBUG) printf("\nTHREAD NODE--> Node %d,%d transporting using south port.",getX(), getY());
				if (NOC_DEBUG) printf("\nTHREAD NODE--> Direction:%d.",ex->getDirection());	

				counterS++; 

				if (ex->getDirection() == FORWARD) {
					
					
					status = S_init_socket->nb_transport_fw(payload,phase,time_info);
					if (status != tlm::TLM_UPDATED)
					{
						printf("\nTHREAD NODE--> Node %d,%d TRANSPORT ERROR (1)", getX(), getY());
						exit(1);
					}	
				}
				else {
					
					status = S_target_socket->nb_transport_bw(payload,phase,time_info);
					if (status != tlm::TLM_COMPLETED)
					{
						printf("\nTHREAD NODE--> Node %d,%d TRANSPORT ERROR (2)", getX(), getY());
						exit(1);
					}						


				}

			}
			if (NOC_DEBUG) printf("\nTHREAD NODE--> Thread of node %d,%d is calling wait (1)\n",getX(), getY());
			wait(1,SC_NS);
		}
		
		if (NOC_DEBUG) printf("\nTHREAD NODE--> Thread of node %d,%d is calling wait(event) (2)\n",getX(), getY());
		
		wait(this->wake_up);
		if (NOC_DEBUG) printf("\nTHREAD NODE--> Thread of node %d,%d is waking up\n",getX(), getY());
		
		
	}

}


void tlm_node::addToBuffer (ac_tlm2_payload &p, tlm::tlm_phase& phase, sc_core::sc_time &t)
{

	
	if (numberOfPackagesInBuffer == SIZE_OF_BUFFER_PACKAGES)
	{
		if (NOC_DEBUG) printf("\nADD INTO BUFFER --> Node %d,%d is trying to add package into the buffer.",getX(), getY());
		printf("\nWARNING --> Missing a package! There's no buffer space to store it.");
		
	}
	else
	{
	

		packageType *pack = new packageType(p,phase,t);

		if (numberOfPackagesInBuffer == 0){
			beginListPackage = pack;
		} 
		else endListPackage->next = pack;
		
		endListPackage = pack;
		numberOfPackagesInBuffer++;

		#ifdef POWER_SIM
    	ps->update_stat_power(1);
		#endif


		
		if (NOC_DEBUG)
 		{	
			printf("\nADD INTO BUFFER--> Node %d,%d is adding package into - the buffer has %d packages",getX(), getY(), getNumberOfPackagesInBuffer());
		}
		
	}

}


void tlm_node::removeFromBuffer (ac_tlm2_payload &p, tlm::tlm_phase& phase, sc_core::sc_time &t)
{

	
	if (getNumberOfPackagesInBuffer() == 0)
	{
		printf("\nREMOVE FROM BUFFER--> ERROR--> There's no package to remove.");
		exit(1);	
	}
	else
	{
		
		
		unsigned char* data_pointer = beginListPackage->payload.get_data_ptr();		
		p.set_data_ptr(data_pointer);

		p.deep_copy_from(beginListPackage->payload);

		t = beginListPackage->time;
		phase = beginListPackage->phase;

		packageType *aux = beginListPackage;
		beginListPackage = beginListPackage->next;

		numberOfPackagesInBuffer--;
		delete aux;

		if (numberOfPackagesInBuffer == 0)
		{
			beginListPackage = endListPackage = NULL;
		}
		if (NOC_DEBUG) printf("\nREMOVE FROM BUFFER--> Node %d,%d is removing package - the buffer has %d packages",getX(), getY(),getNumberOfPackagesInBuffer());

		
	}
}



void tlm_node::b_transport(ac_tlm2_payload& payload, sc_core::sc_time& time_info){


	if (getStatus() == OFF)
	{
		printf("\nERROR--> Transport method in a node with status OFF ");
		exit(1);
	}


	tlm_payload_extension *ex;	
  	payload.get_extension(ex);
	

	
	if (getX() == ex->getTargetX())
	{
		if (getY() == ex->getTargetY())
		{

			if (NOC_DEBUG) printf("\nTransporting to the wrapper using LOCAL port. \n");
			payload.clear_extension(ex); // we can not use clear_extension (1) because is private

			LOCAL_init_socket->b_transport(payload,time_info);
		}
		else if ( getY() > ex->getTargetY() )
		{
			
			if (NOC_DEBUG) printf("\nB_TRANSPORT--> Node %d,%d transporting using west port.",getX(), getY());
			W_init_socket->b_transport(payload,time_info);
			
		}
		else  {

			if (NOC_DEBUG) printf("\nB_TRANSPORT--> Node %d,%d transporting using east port.",getX(), getY());
			E_init_socket->b_transport(payload,time_info);
		}
	}
	else if (getX() > ex->getTargetX())
	{
		if (NOC_DEBUG) printf("\nB_TRANSPORT--> Node %d,%d transporting using north port.",getX(), getY());
		N_init_socket->b_transport(payload,time_info);
	}
	else {
		if (NOC_DEBUG) printf("\nB_TRANSPORT--> Node %d,%d transporting using south port.",getX(), getY());
		S_init_socket->b_transport(payload,time_info);
	}

	if (NOC_DEBUG)
    	{	
	    printf("\nB_TRANSPORT returning");
   	}

}


tlm_slave_node::tlm_slave_node(sc_module_name mod_name):
  sc_module(mod_name),
  LOCAL_slave_target_socket()
  {
	LOCAL_slave_target_socket.register_nb_transport_fw(this, &tlm_slave_node::nb_transport_fw);

  } 

tlm_slave_node::tlm_slave_node():
  module_name("slave_node"),
  sc_module( (sc_module_name)module_name ),
  LOCAL_slave_target_socket()
  {
	LOCAL_slave_target_socket.register_nb_transport_fw(this, &tlm_slave_node::nb_transport_fw);
	
  } 


tlm_master_node::tlm_master_node(sc_module_name mod_name):
  sc_module(mod_name),
  LOCAL_master_init_socket()
  {
	LOCAL_master_init_socket.register_nb_transport_bw(this, &tlm_master_node::nb_transport_bw);
        
  } 

tlm_master_node::tlm_master_node():
  module_name("master_node"),
  sc_module((sc_module_name)module_name ),
  LOCAL_master_init_socket()
  {
	LOCAL_master_init_socket.register_nb_transport_bw(this, &tlm_master_node::nb_transport_bw);
	
  } 

packageType::packageType(ac_tlm2_payload &p, tlm::tlm_phase& ph, sc_core::sc_time &t)
{

				/* TESTTTTT */
				payload.free_all_extensions();

				unsigned char *data_pointer = p.get_data_ptr();
				payload.set_data_ptr(data_pointer);

				payload.deep_copy_from(p);

				time = t;
				phase = ph;
				next = NULL;
				prev = NULL;
}


packageType::~packageType()
{
		payload.free_all_extensions();
}