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
 * @brief     Defines wrappers for TLM 2.0 NoC
 *
 *
 *******************************************************************************/



#include "wrappers_noc.h"

using user::wrapper_master_slave_to_noc;
using user::tlm_payload_extension;
using tlm::tlm_extension_base;


//using user::wrapper_master_to_noc;
//using user::wrapper_slave_to_noc;

wrapper_master_slave_to_noc::wrapper_master_slave_to_noc():
  module_name("wrapper_master_slave"),
  sc_module((sc_module_name)module_name),
  LOCAL_init_socket(),
  LOCAL_target_socket(),
  NODE_init_socket(),
  NODE_target_socket()
  {
	#ifdef POWER_SIM
  	dfs = NULL;
  	#endif

	/* NUMBER OF HOPS */
	numberOfHops = 0;
	numberOfPackages = 0;
	/* NUMBER OF HOPS */

	LOCAL_init_socket.register_nb_transport_bw(this, &wrapper_master_slave_to_noc::nb_transport_bw);
	LOCAL_target_socket.register_nb_transport_fw(this, &wrapper_master_slave_to_noc::nb_transport_fw);

	NODE_init_socket.register_nb_transport_bw(this, &wrapper_master_slave_to_noc::nb_transport_bw);
	NODE_target_socket.register_nb_transport_fw(this, &wrapper_master_slave_to_noc::nb_transport_fw);

	setStatus(OFF);

  }



wrapper_master_slave_to_noc::wrapper_master_slave_to_noc(sc_module_name module_name) :
  sc_module( module_name ),
  LOCAL_init_socket(),
  LOCAL_target_socket(),
  NODE_init_socket(),
  NODE_target_socket()
  {

    #ifdef POWER_SIM
	dfs = NULL;
	#endif
	/* NUMBER OF HOPS */
	numberOfHops = 0;
	numberOfPackages = 0;
	/* NUMBER OF HOPS */

	LOCAL_init_socket.register_nb_transport_bw(this, &wrapper_master_slave_to_noc::nb_transport_bw);
	LOCAL_target_socket.register_nb_transport_fw(this, &wrapper_master_slave_to_noc::nb_transport_fw);

	NODE_init_socket.register_nb_transport_bw(this, &wrapper_master_slave_to_noc::nb_transport_bw);
	NODE_target_socket.register_nb_transport_fw(this, &wrapper_master_slave_to_noc::nb_transport_fw);


	setStatus(OFF);

	
  }


tlm::tlm_sync_enum wrapper_master_slave_to_noc::nb_transport_fw(ac_tlm2_payload& payload, tlm::tlm_phase& phase, sc_core::sc_time& time_info)
{

		
	tlm::tlm_sync_enum status;

	if (NOC_DEBUG) printf("\n\nNB_TRANSPORT_FW--> Wrapper %d,%d with status %d is receiving a package",getX(), getY(), getStatus());
 	
	
    tlm_payload_extension *ex;
  	tlm::tlm_extension_base* base;
   	base = payload.get_extension(1);

   	ex = reinterpret_cast<tlm_payload_extension*>(base);

 	//tlm::tlm_extension_base* teste = payload.get_extension(0);
 	//if (teste == 0) printf("\nwrapper nao deu certo a extensao 0 ");
    //else   printf("\nwrapper - deu certo a extensao 0 (de dir)");

	// payload.get_extension(ex);
	

	if (ex==NULL)
	{
		
		if (NOC_DEBUG) printf("\nNB_TRANSPORT_FW-->Wrapper %d,%d is creating payload extension",getX(),getY());
		ex = new tlm_payload_extension();

		uint64_t addr = payload.get_address();
		int targetX, targetY;
		tableOfRouts.returnsTargetPosition(addr, targetX, targetY);

		ex->setTargetX(targetX);
		ex->setTargetY(targetY);
		ex->setInitX(getX());
		ex->setInitY(getY());
		ex->setDirection(FORWARD);

		/* NUMBER OF PACKAGES */
		this->numberOfPackages ++;
		if (NOC_DEBUG) printf("\nNB_TRANSPORT_FW --> Wrapper %d,%d is incrementing the number of requests(%ld)",getX(), getY(),this->numberOfPackages); 	   			


		if (NOC_DEBUG) printf("\nNB_TRANSPORT_FW --> Wrapper %d,%d is setting the new extension into the payload",getX(),getY()); 	   	
		if (NOC_DEBUG) printf("\nNB_TRANSPORT_FW --> Wrapper %d,%d is setting firstForward with false into the payload extension",getX(),getY());
		if (NOC_DEBUG) printf("\nNB_TRANSPORT_FW-->Wrapper %d,%d is processing transaction from InitX-> %d InitY-> %d to TargetX-> %d TargetY-> %d", getX(), getY(), ex->getInitX(), ex->getInitY(), ex->getTargetX(), ex->getTargetY());				

		//payload.set_extension(ex);
		payload.set_extension(1,ex);
	}

	if (ex->isFirstForward())
	{	
		ex->setFirstForward(false);	
		if (NOC_DEBUG) printf("\nNB_TRANSPORT_FW--> Wrapper %d,%d is trasporting package INTO the NOC using non-blocking-forward transport trought NODE-init-socket", getX(), getY());

		status = NODE_init_socket->nb_transport_fw(payload,phase,time_info); 
		if (status != tlm::TLM_UPDATED)
		{
			printf("\nNB_TRANSPORT_FW--> Wrapper %d,%d TRANSPORT ERROR (2)", getX(), getY());
			exit(1);
		}

		#ifdef POWER_SIM
		#ifdef DFS_AUTO_SELECTION_CPU_RATE
		dfs->noteFwTime();
		#endif
		#endif
	

		//payload.release_extension(1);
	}
	else
	{

		if (NOC_DEBUG)  printf("\nNB_TRANSPORT_FW--> Wrapper %d,%d is trasporting package out of NOC to the target using blocking-forward transport trought LOCAL-init-socket", getX(), getY());
		LOCAL_init_socket->b_transport(payload,time_info);
		if (payload.get_response_status() != tlm::TLM_OK_RESPONSE)
		{
			printf("\nNB_TRANSPORT_FW--> Wrapper %d,%d TRANSPORT ERROR (1)", getX(), getY());
			exit(1);
		}

		if (NOC_DEBUG) printf("\nNB_TRANSPORT_FW--> Wrapper %d,%d is transporting into the NOC using non-blocking-backward transport trought the NODE-target-socket",getX(), getY());
		status = this->nb_transport_bw(payload,phase,time_info);
		if (status != tlm::TLM_COMPLETED)
		{
			printf("\nNB_TRANSPORT_FW--> Wrapper %d,%d TRANSPORT ERROR (2)", getX(), getY());
			exit(1);
		}
	
	}

	

	if (NOC_DEBUG) printf("\n\nNB_TRANSPORT_FW --> Wrapper %d,%d is returning with status %d\n",getX(), getY(),tlm::TLM_UPDATED);
	phase = tlm::END_REQ;
	return status;
}


tlm::tlm_sync_enum wrapper_master_slave_to_noc::nb_transport_bw(ac_tlm2_payload& payload, tlm::tlm_phase& phase, sc_core::sc_time& time_info)
{

	if (NOC_DEBUG) printf("\n\nNB_TRANSPORT_BW --> Wrapper %d,%d with status %d is receiving a package",getX(), getY(), getStatus());
 	
	tlm::tlm_sync_enum status;

	tlm_payload_extension *ex;
  	tlm::tlm_extension_base* base;
   	base = payload.get_extension(1);

   	ex = reinterpret_cast<tlm_payload_extension*>(base);

	//payload.get_extension(ex);
	
	
	if (ex == NULL)  {
		printf("\n\nNB_TRANSPORT_BW ERROR --> Wrapper %d,%d is processing a NULL payload extension",getX(),getY());
		exit(1);
	}

	if (ex->isFirstBackward())
	{
		ex->setFirstBackward(false);
		ex->setDirection(BACKWARD);
		ex->exchangeXY();
		phase = tlm::TLM_COMPLETED;

		if (NOC_DEBUG) printf("\n\nNB_TRANSPORT_BW --> Wrapper %d,%d is setting firstBackward with false into the payload extension",getX(),getY());
		if (NOC_DEBUG) printf("\nNB_TRANSPORT_BW--> Wrapper %d,%d is trasporting package INTO the NOC using non-blocking-backward transport trought NODE-target-socket", getX(), getY());
		
		status = NODE_target_socket->nb_transport_bw(payload,phase,time_info); 
		if (NOC_DEBUG) printf("\n\nNB_TRANSPORT_BW --> Wrapper %d,%d is returning\n",getX(), getY());
		phase = tlm::END_RESP;
		return status;



	}
	else
	{


		//printf("\ncpu_wait_time: %.10lf", cpu_wait_time);

		if (NOC_DEBUG) printf("\nNB_TRANSPORT_BW--> Wrapper %d,%d is cleaning payload extension", getX(), getY());
		
		/*NUMBER OF HOPS*/
		this->numberOfHops += ex->getNumberOfHops();
		if (NOC_DEBUG) printf("\nNB_TRANSPORT_FW --> Wrapper %d,%d is updating the number of hops (%ld)",getX(), getY(),this->getNumberOfHops());
		/*NUMBER OF HOPS*/

		// TESTE
		//payload.release_extension(ex);
		//payload.release_extension(1);

		if (NOC_DEBUG) printf("\nNB_TRANSPORT_BW--> Wrapper %d,%d is trasporting package OUT of NOC using non-blocking-backward transport trought LOCAL-target-socket", getX(), getY());
		status = LOCAL_target_socket->nb_transport_bw(payload,phase,time_info); 
		if (status != tlm::TLM_COMPLETED)
		{
			printf("\nNB_TRANSPORT_BW--> Wrapper %d,%d TRANSPORT ERROR", getX(), getY());
			exit(1);
		}
		phase = tlm::END_RESP;


		#ifdef POWER_SIM
		#ifdef DFS_AUTO_SELECTION_CPU_RATE
		dfs->noteBwTime();
		dfs->autoSelectionCPUrate();
		#endif
		#ifdef DFS_AUTO_SELECTION_ENERGY_STAMP
		dfs->autoSelectionEnergyStamp();
		#endif
		#endif

		return status;
	}

	
}


#ifdef POWER_SIM
void wrapper_master_slave_to_noc::initDFS (PROCESSOR_NAME* proc)
{
	
    dfs = new local_dfs("dfs",proc);
    
}
#endif