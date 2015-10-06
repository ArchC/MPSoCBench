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
 * @file      tlm_noc_events.h
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
 * @brief     Defines a TLM 2.0 NOC
 *
 *******************************************************************************/


#ifndef TLM_NOC_EVENTS_H_
#define TLM_NOC_EVENTS_H_

//////////////////////////////////////////////////////////////////////////////

#include <systemc>
#include "../../defines.h"

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////


namespace user
{

class tlm_noc_events:
{
public:

	tlm_noc_events (int lines,int columns)
	{
		this->lines = lines;
		this->columns = columns;

		events = new sc_event*[this->lines];
		for (int i=0; i<this->lines; i++)
		{
		     	events[i] = new sc_event[this->columns];

		}
	}

	~tlm_noc_events ()
	{
		for (int i=0; i<this->lines; i++)
			delete [] events[i];

		delete [] events;
	}

	sc_event **events;
	int lines;
	int columns;	
  
};

};

#endif 
