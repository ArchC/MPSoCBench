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


#include "tlm_noc.h"
#include "tlm_node.h"
#include "wrapper_noc.h"

/// Namespace to isolate noc from ArchC
using user::tlm_noc;
using user::tlm_empty_master_node;
using user::tlm_empty_slave_node;
using user::wrapper_noc;

int tlm_noc::numberOfMasterEmptyNodes = 0;

/// Constructor
tlm_noc::tlm_noc( sc_module_name module_name, int n, int m,int lines, int columns) :
  sc_module( module_name )
  {

	setNumberOfMasters(n);
	setNumberOfSlaves(m);
	setNumberOfPeripherals(n+m);
	setNumberOfLines(lines);
	setNumberOfColumns(columns);
	setNumberOfNodes(getNumberOfLines()*getNumberOfColumns()); 
        setNumberOfWrappers(getNumberOfNodes());
	setNumberOfInactiveNodes( getNumberOfNodes()-getNumberOfPeripherals() ); 
	create();
	init();
	bindingInternalPorts();
	

  }

void tlm_noc::create()
{
	mesh = new tlm_node*[numberOfLines];
	
	for (int i=0; i<numberOfLines; i++)
        {
		mesh[i] = new tlm_node[numberOfColumns];
	}

	wrapper = new wrapper_noc[getNumberOfWrappers()];
}



tlm_noc::~tlm_noc()
{
	for (int i=0; i<numberOfLines; i++)
		delete [] mesh[i];

	delete [] mesh;

        delete [] wrapper;
}


void tlm_noc::init()
{

	// Nodes
	for (unsigned int i=0; i<numberOfLines; i++)
		for (unsigned int j=0; j<numberOfColumns; j++)
			mesh[i][j].setStatus(OFF);
		
	// Wrappers
	for (unsigned int i=0; i<getNumberOfWrappers(); i++)
	{
		wrapper[i].setStatus(OFF);
	}
}


int tlm_noc::getNodeIndex(int i, int j)
{
	return mesh[i][j].getId();
}
 

void tlm_noc::bindingInternalPorts()
{


	unsigned int wr = 0;

	for (int i=0; i<numberOfLines; i++)
	{	for (int j=0; j<numberOfColumns; j++)
		{	
	
			// status
			if (wr < getNumberOfPeripherals()) { 
				mesh[i][j].setStatus(ON);
				wrapper[wr].setStatus(ON);
			}

			// position
			wrapper[wr].setX(i);
			wrapper[wr].setY(j);
			mesh[i][j].setX(i);
			mesh[i][j].setY(j);


			// bind node and wrapper
			mesh[i][j].LOCAL_port(wrapper[wr].LOCAL_export);
        		wrapper[wr].NODE_port(mesh[i][j].LOCAL_export);	
			wr++;

			if (j<numberOfColumns-1) mesh[i][j].E_port(mesh[i][j+1].LOCAL_export);
			else 	mesh[i][j].E_port(edgesEmptyNode.LOCAL_export);

			if (j>0) mesh[i][j].W_port(mesh[i][j-1].LOCAL_export);
			else 	mesh[i][j].W_port(edgesEmptyNode.LOCAL_export);

			if (i>0) mesh[i][j].N_port(mesh[i-1][j].LOCAL_export);
			else 	mesh[i][j].N_port(edgesEmptyNode.LOCAL_export);

			if (i<numberOfLines-1) mesh[i][j].S_port(mesh[i+1][j].LOCAL_export);
			else 	mesh[i][j].S_port(edgesEmptyNode.LOCAL_export);
		}
	}
}

//void tlm_noc::bindingEmptyMaster(int i, int j)
//{
	
	//masterEmptyNodes[numberOfMasterEmptyNodes++].LOCAL_port(wr[i][j].LOCAL_export);
					
	
//}


void tlm_noc::bindingEmptySlave(int i)
{
	wrapper[i].LOCAL_port(edgesEmptyNode.LOCAL_export);
        
}
void tlm_noc::preparingRoutingTable()
{
	for (int i=0; i<getNumberOfWrappers(); i++)
		wrapper[i].tableOfRouts.copyFrom(tableOfRouts);
}

inline void tlm_noc::setNumberOfMasters(int n)
{
	this->numberOfMasters = n;
}
inline void tlm_noc::setNumberOfSlaves(int m)
{
	this->numberOfSlaves = m;
}
inline void tlm_noc::setNumberOfPeripherals(int n)
{
  	this->numberOfPeripherals = n;
}
inline void tlm_noc::setNumberOfInactiveNodes(int n)
{
  	this->numberOfInactiveNodes = n;
}
inline void tlm_noc::setNumberOfLines(int n)
{
  	this->numberOfLines = n;
} 
inline void tlm_noc::setNumberOfColumns(int n)
{
  	this->numberOfColumns = n;
}

inline void tlm_noc::setNumberOfNodes(int n)
{
  	this->numberOfNodes = n;
}
inline void tlm_noc::setNumberOfWrappers(int n)
{
  	this->numberOfWrappers = n;
}
 
inline int tlm_noc::getNumberOfMasters()
{
	return this->numberOfMasters; 
} 
inline int tlm_noc::getNumberOfSlaves()
{
	return this->numberOfSlaves; 
} 

inline int tlm_noc::getNumberOfPeripherals()
{
	return this->numberOfPeripherals; 
} 
  
inline int tlm_noc::getNumberOfInactiveNodes()
{
	return this->numberOfInactiveNodes; 
} 
int tlm_noc::getNumberOfLines()
{
	return this->numberOfLines; 

}
int tlm_noc::getNumberOfColumns()
{
	return this->numberOfColumns; 
}

int tlm_noc::getNumberOfNodes()
{
	return this->numberOfNodes; 
}
int tlm_noc::getNumberOfWrappers()
{
	return this->numberOfWrappers; 
}

void tlm_noc::print ()
{
	printf("\nNOC:\n");
	printf("\n%d Lines\n",getNumberOfLines());
	printf("\n%d Columns\n",getNumberOfColumns());
        printf("\n%d Peripherals\n",getNumberOfPeripherals());
        printf("\n%d Masters\n", getNumberOfMasters());
        printf("\n%d Slaves\n", getNumberOfSlaves());
	printf("\n%d Inactive Nodes\n", getNumberOfInactiveNodes());
	printf("\n%d Wrappers\n", getNumberOfWrappers());

	int k = getNumberOfLines();
	int m = getNumberOfColumns();

	printf("\nPrinting mesh!\n");
	for(int i=0; i<k; i++)
	{
		for (int j=0; j<m; j++)
		{
			printf("\nmesh[%d][%d]: id->%d status-> %d  x->%d  y->%d",i,j,mesh[i][j].getId(),mesh[i][j].getStatus(),mesh[i][j].getX(),mesh[i][j].getY());
			
		}
	}


        printf("\nPrinting Table of routs!");
	tableOfRouts.print();       


}




