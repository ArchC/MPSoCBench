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
 * @file      tlm_payload_dir_extension.h
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
 * @brief     Defines the payload extension useful to the non-blocking ArchC 
 *            TLM 2.0  port.
 *
 *
 *******************************************************************************/


#ifndef TLM_PAYLOAD_EXTENSION_H
#define TLM_PAYLOAD_EXTENSION_H

#include <tlm.h>

#include "../../defines.h"

namespace user {

class tlm_payload_dir_extension : public tlm::tlm_extension<tlm_payload_dir_extension>
	{
		public:
			tlm_payload_dir_extension(){
				numberCache = 0;	
				address = 0;
				cacheIndex=0;
				validation = false;
				nWay = 0;
				index_size = 0;
			}
			~tlm_payload_dir_extension() {
			}
			
			int getnumberCache () { return numberCache; }
			uint32_t getAddress () { return address; }
			int getCacheIndex(){return cacheIndex;}
			bool getValidation(){return validation;}
			int getNWay () { return nWay;}
			int getIndex_size () { return index_size; }
			
			void setRule(int a){rule =  a; }
			int getRule(){return rule; }
			
			void setNumberCache (int a) { numberCache = a; }
			void setAddress (uint32_t b) { address = b; }
			void setCacheIndex (int index){cacheIndex= index;}
			void setValidation (bool validate){validation = validate;}
			void setNWay (int n) { nWay = n; }
			void setIndex_size (int index) { index_size = index; }
			
			virtual tlm_extension_base* clone() const {
			
				tlm_payload_dir_extension *ext = new tlm_payload_dir_extension();
				ext->numberCache = numberCache;
				ext->cacheIndex = cacheIndex; 
				ext->address = address; 
				ext->rule = rule;
				ext->validation = validation;
				ext->nWay = nWay;
				ext->index_size = index_size;
				return ext;

			}
			virtual void copy_from(tlm_extension_base const &ext) {  

				printf("\nPayload extension copy_from() isn't implemented - clone() can be used intead, if necessary ");
				exit(1);
				
			} 
			
		private: 
			int numberCache;
			uint32_t address;
			int cacheIndex;
			int rule; //1 representa read, 2 representa write
			bool validation;
			int nWay;
			int index_size;
	};


};
/***********************************************************/



#endif
