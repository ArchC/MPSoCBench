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


// File: ESLDiagram.h


#ifndef _ESLDIAGRAM_H
#define _ESLDIAGRAM_H

#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <inttypes.h>

#include  <systemc.h>
#include "/home/liana/repository/oficial/tools/systemc/src/sysc/kernel/sc_simcontext.h"
#include "/home/liana/repository/oficial/tools/systemc/src/sysc/kernel/sc_module_registry.h"
#include "/home/liana/repository/oficial/tools/systemc/src/sysc/kernel/sc_object.h"
#include "/home/liana/repository/oficial/tools/systemc/src/sysc/communication/sc_port.h"
#include "/home/liana/repository/oficial/tools/systemc/src/sysc/communication/sc_interface.h"
#include "/home/liana/repository/oficial/tools/systemc/src/sysc/communication/sc_export.h"
#include "ComponentList.h"

using std::cout;
using std::endl;

class ESLDiagram
{
	public:
		ESLDiagram (sc_simcontext* my_s)
		{
			my_sim = my_s;
			//my_module_registry = my_sim->get_module_registry();
			//my_module_registry_size = my_module_registry->size();
		}

        // atributos

		sc_simcontext* my_sim;
		//sc_module_registry* my_module_registry;
		//int my_module_registry_size;
	    const ::std::vector<sc_object*> my_child_objects;

	    ComponentList listOfObjects;

	    void startCapture()
	    {
	    	this->captureObjects();
	    	this->excRepetedInterfaces();
	    	this->captureRelations();
	    	this->print();

	    }


		void captureObjects();
		void recursiveDescent( sc_object* );
		void print();
		void captureRelations();
		void excRepetedInterfaces();


};

void ESLDiagram::captureObjects()
{

  	  std::vector<sc_object*> tops = sc_get_top_level_objects();
	  

  	  char resp;
  	  sc_port_base *mport;
  	  sc_export_base *mexp;
  	  sc_module *mmod;
  	  sc_clock *mclock;

  	  sc_interface *miface;

  	  for ( unsigned i = 0; i < tops.size(); i++ )
  	  {
  		   ComponentType t (tops[i]->name(),tops[i]->kind());


  		 const char *nameComponent = t.name;
  		 sc_object *obj = sc_find_object(nameComponent);


  		 if (miface = dynamic_cast<sc_interface*>(obj))
  		 {
  		         t.interface = (long unsigned int) miface;
  		 }

  		 else if (mport = dynamic_cast<sc_port_base*>(obj))
  		 {
  			     sc_interface* iface = mport->get_interface();
  			     t.interface = (long unsigned int) iface;
  		 }

  		 else if (mexp = dynamic_cast<sc_export_base*>(obj))
  		 {
  			 	 sc_interface* iface = mexp->get_interface();
  				 t.interface = (long unsigned int) iface;

  		 }
  		 else if (mmod = dynamic_cast<sc_module*>(obj))
  		 {
  			   	t.interface = (long unsigned int) mmod;
  		 }
  		 else if (mclock = dynamic_cast<sc_clock*>(obj))
  		 {
  			 	sc_interface* iface = mclock;
  		  		t.interface = (long unsigned int) iface;

  		 }
  		 else
  		 {
  			   cout << "\nESLDiagram warning...dynamic_cast for some module isn't implemented ";

  		 }

  		 listOfObjects.add(t);


 		 recursiveDescent( tops[i] );
  	  }

}
void ESLDiagram::recursiveDescent( sc_object* obj )
{
  	  char resp;
		std::vector<sc_object*> children = obj->get_child_objects();

  	  for ( unsigned i = 0; i < children.size(); i++ )
  	  {
  		  ComponentType t (children[i]->name(),children[i]->kind());
  		  ComponentNode* node = new ComponentNode(t);
  		  ComponentNode* tmp = listOfObjects.getTail()->headOfChildrenList;
  		  

  		  node->next = tmp;
  		  node->prev = 0;
		
		  const char *nameComponent = t.name;

		  sc_object *obj = sc_find_object(nameComponent);
          sc_port_base *mport;
		  sc_export_base *mexp;
		  sc_module *mmod;
		  sc_clock *mclock;
		  sc_interface *miface;

          //cout << "\nComponente: " << t.name;

          if (miface = dynamic_cast<sc_interface*>(obj))
          {
           		 (node->info).interface = (long unsigned int) miface;
          }

          else if (mport = dynamic_cast<sc_port_base*>(obj))
	  	  {
			     sc_interface* iface = mport->get_interface();
			     (node->info).interface = (long unsigned int) iface;

		  }

		  else if (mexp = dynamic_cast<sc_export_base*>(obj))
	  	  {

			     sc_interface* iface = mexp->get_interface();	
			     (node->info).interface = (long unsigned int) iface;


	  	  }
		  else if (mmod = dynamic_cast<sc_module*>(obj))
		  {
			    sc_interface* iface = (sc_interface*) mmod;
			   	(node->info).interface = (long unsigned int) iface;


		  }
		  else if (mclock = dynamic_cast<sc_clock*>(obj))
	  	  {
	  		 	sc_interface* iface = mclock;
	  	  		(node->info).interface = (long unsigned int) iface;

	  	  }
		  else
		  {
 			    //cout << "\nESLDiagram warning...dynamic_cast for some module isn't implemented ";

		  }

		  listOfObjects.getTail()->headOfChildrenList = node;
		  listOfObjects.incNumberOfComponents();
		  recursiveDescent( children[i] );
  	  }

}


void ESLDiagram::excRepetedInterfaces()
{
		this->listOfObjects.excRepetedInterfaces();
}



void ESLDiagram::captureRelations()
{

	  this->listOfObjects.captureRelations();
}




void ESLDiagram::print()
{
	listOfObjects.printAtFile();
	listOfObjects.print();
}


#endif


