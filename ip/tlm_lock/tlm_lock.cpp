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


#include "tlm_lock.h"

// If you don't want to measure lock access, modify measures_lock to 0
#define measures 1

long  unsigned int count_lock = 0;
FILE *local_lock_file;
FILE *global_lock_file;

using user::tlm_lock;
using tlm::tlm_command;
using tlm::TLM_READ_COMMAND;
using tlm::TLM_WRITE_COMMAND;

tlm_lock::tlm_lock(sc_module_name module_name) :
  sc_module( module_name ),
  target_export("iport")
{
    /// Binds target_export to the memory
    target_export( *this );
    value = 0;
}

/// Destructor
tlm_lock::~tlm_lock() {
  
 if (measures) 
  {
     local_lock_file = fopen (LOCAL_FILE_MEASURES_NAME,"a");
     global_lock_file = fopen (GLOBAL_FILE_MEASURES_NAME,"a");
     fprintf(local_lock_file, "\nLock Access:\t%ld", count_lock);
     fprintf(global_lock_file, "\nLock Access:\t%ld", count_lock);
     fclose (local_lock_file);
     fclose (global_lock_file);
  }

}

uint32_t tlm_lock::readm(uint32_t &d )
{


        if(d != 0x0)
		return 0x1;

	else
		return d;

}

void tlm_lock::b_transport( ac_tlm2_payload &payload, sc_core::sc_time &time_info ) {


   time_info = time_info + sc_core::sc_time(1,SC_NS);
   
   count_lock ++;

   tlm_command command = payload.get_command();
   uint32_t addr = (uint32_t) payload.get_address();
  
   unsigned char* data_pointer = payload.get_data_ptr();
  
   unsigned int len = payload.get_data_length();
   
   unsigned int resp;

   payload.set_response_status(tlm::TLM_OK_RESPONSE);    
   
   switch(command) {

	case TLM_READ_COMMAND :     // Packet is a READ one
	
		if (readm(value)!=0) resp = 1;
		else resp = 0;

		if (len == 1) *((uint8_t*)data_pointer) = (uint8_t) resp;  
		else if (len == 2) *((uint16_t*)data_pointer) = (uint16_t) resp;  			
		else if (len == 4) *((uint32_t*)data_pointer) = (uint32_t)resp;
		else if (len == 8) *((uint64_t*)data_pointer) = (uint64_t) resp;
		else {
			printf("\nLOCK READ ERROR --> wordsize not implemented"); 
			exit(0); 
		}
		
		if(debugTLM2)	printf("\nTLM LOCK B_TRANSPORT: read response--> %d value-->%d --> %d",resp,value,1);

		
		value = 0x1;
		
                break; 

	case TLM_WRITE_COMMAND:     // Packet is a WRITE
	        

               	if (*((uint32_t*)data_pointer)==0 ) 
			value = 0;
		else 
			value = 1;

		if(debugTLM2) printf("\nTLM LOCK B_TRANSPORT: write %d em value    *data_pointer-->%d",value,*((uint32_t*)data_pointer));
	
    		break;
    default:
		printf("LOCK TRANSPORT --> COMMAND %d IS NOT IMPLEMENTED", command);
      		break; 
    }

    
  }




