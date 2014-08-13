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
 * @file      tlm_payload_extension.h
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

class tlm_payload_extension : public tlm::tlm_extension<tlm_payload_extension>
	{
		public:
			tlm_payload_extension(){
				targetX = 0;	
				targetY = 0;		
				initX = 0;
				initY = 0;
				firstBackward = true;
				firstForward = true;
				direction = FORWARD;

				/*NUMBER OF HOPS*/					
				numberOfHops = 0;
				/*NUMBER OF HOPS*/					

			}
			~tlm_payload_extension() {
			}
			
			int getTargetX () { return targetX; }
			int getTargetY () { return targetY; }
			int getInitX () { return initX; }
			int getInitY () { return initY; }
			
			void setTargetX (int a) { targetX = a; }
			void setTargetY (int b) { targetY = b; }
			void setInitX (int a) { initX = a; }
			void setInitY (int b) { initY = b; }
			
			void setDirection (int a) { direction = a; }
			int getDirection  () { return direction; }
	
			void exchangeXY(){
				int temp = targetX;
				targetX = initX;
				initX = temp;
				temp = targetY;
				targetY = initY;
				initY = temp;
			}

			
			void incNumberOfHops() { numberOfHops++; }
			int getNumberOfHops () {return numberOfHops;}


			void setFirstBackward (bool v) { firstBackward = v; }
			bool getFirstBackward () { return firstBackward; }
			void setFirstForward (bool v) { firstForward = v; }
			bool getFirstForward () { return firstForward; }

			bool isFirstForward () { return firstForward;  }
			bool isFirstBackward() { return firstBackward; }
			
			
			virtual tlm_extension_base* clone() const {
			
				tlm_payload_extension *ext = new tlm_payload_extension();
				ext->targetX = targetX; 
				ext->targetY = targetY; 
				ext->initX = initX;
				ext->initY = initY;
				ext->firstForward = firstForward;
				ext->firstBackward = firstBackward;
				ext->direction = direction;
				ext->numberOfHops = numberOfHops;
				
				return ext;

			}
			virtual void copy_from(tlm_extension_base const &ext) {  

				printf("\nPayload extension copy_from() isn't implemented - clone() can be used intead, if necessary ");
				exit(1);
				
			} 
			
		private: 
			int targetX;
			int targetY;
			int initX;
			int initY;
			int direction;

			bool firstBackward;
			bool firstForward;

			/*NUMBER OF HOPS*/	
			int numberOfHops;
			/*NUMBER OF HOPS*/
	};


};
/***********************************************************/



#endif
