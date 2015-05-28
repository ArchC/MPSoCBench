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

const char *project_name="platform.noc.at";
const char *project_file="";
const char *archc_version="";
const char *archc_options="";

#include  <systemc.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../defines.h"
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include  "tlm_memory.h"
#include  "tlm_noc.h"
#include  "tlm_lock.h"
#include  "wrappers_noc.h"
#include  "tlm_dfs.h"

#include "ac_utils.H"
#include "ac_tlm_protocol.H"
#include "tlm_intr_ctrl.h"
#include "tlm_dir.h"

//#define AC_DEBUG

using user::tlm_memory;
using user::tlm_noc;
using user::tlm_lock;
using user::wrapper_master_slave_to_noc;
using user::tlm_intr_ctrl;
using user::tlm_dir;
using user::tlm_dfs;



// Global variables
int N_WORKERS;	
struct timeval startTime;
struct timeval endTime;
FILE *local_time_measures;
FILE *global_time_measures;


bool first_load;

// Functions
// Functions
void report_start(char*, char*, char*);
void report_end();
void load_elf(PROCESSOR_NAME &, tlm_memory& , char*, unsigned int, unsigned int);


int sc_main(int ac, char *av[])
{

	sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

	/* Checking arguments */
	int N_WORKERS;


	if (ac!=0) {
		N_WORKERS = atoi(av[2]);
		if(N_WORKERS > MAX_WORKERS) {  
			printf ("\nThe amount of processors must be less than 64 %d.\n", MAX_WORKERS);
 			exit(1);
	       	}
		
	}
	else {
		printf("\nNo arguments for main.\n");
		exit(1);
	}


	///************************************************************************************
	//Creating platform components
	//	- A memory with MEM_SIZE bytes    
	//	- A lock device 
	//	- A set of N_WORKERS processors
	//	- A NoC with suficient nodes useful to communicate memory, lock and processors
        //*************************************************************************************	

	
	PROCESSOR_NAME **processors;
   	processors =  (PROCESSOR_NAME **) new PROCESSOR_NAME*[N_WORKERS];

	for (int i=0; i<N_WORKERS; i++){
		char name[10] = "proc"; 
	    char number_str[3];
		sprintf(number_str, "%d", i);
		strcat(name, number_str);
		processors[i] = (PROCESSOR_NAME *) new PROCESSOR_NAME(name);
		(processors[i]->MEM).setProcId(processors[i]->getId());
	}

	// Creates memory, lock and dfs devices 
	tlm_memory mem("mem",0, MEM_SIZE-1);	// memory 
	tlm_lock locker("lock");		// locker
	tlm_intr_ctrl intr_ctrl ("intr_ctrl",N_WORKERS);
	tlm_dir dir ("dir", N_WORKERS);
	tlm_dfs dfs ("dfs", N_WORKERS, processors);				// dfs


	// Binding processors and interruption controller
	for (int i=0; i<N_WORKERS; i++)
    {
       intr_ctrl.CPU_port[i](processors[i]->intr_port);
       dir.CPU_port[i](processors[i]->intr_port);
    }


	// Processor 0 starts simulatino in ON-mode while the other processors are in OFF-mode
	for (int i=1; i<N_WORKERS; i++)
    {
        intr_ctrl.send(i,OFF); // turn off processors 1,..,N_WORKERS-1
    }
    intr_ctrl.send(0,ON);    // turn on processor 0 (master)

	// Creates the NoC with N_WORKERS+2 active nodes 
    // The NoC is defined with a bidimensional array, then some inactive nodes will be also 
	// created . This NoC has N_WORKERS master nodes (connected with processors) and 3 slave 
	// nodes connected with memory, lock and interrrupt controler. 

	// noc constructor parameters:
    // masters = number of processors
	// slaves = 3 (lock, memory and interrupt controler ) or 4 (+ dfs)
	// NumberOfLines and numberOfColumns define the mesh topology
	
	int masters = N_WORKERS;
	int slaves = 5; // mem, lock, intr_ctrl, dir , dfs
	
	int peripherals = masters + slaves;
	int r = ceil(sqrt(peripherals)); 
	

	tlm_noc noc ("noc",N_WORKERS,slaves,r,r);			


	//***************************************************************
	//  Binding platform components
        //*****************************************************************

    // Connecting memory with the node [0][0] of NoC //
    // noc.mesh[0][0].LOCAL_init_socket.bind(mem.LOCAL_target_socket);

	int wrMS = 0;
	int emptyMasters = 0;
	int emptySlaves = 0;
	int column=0, line=0;


	// NoC [0][0] -> MEM
	noc.wrapper[wrMS].LOCAL_init_socket.bind(mem.LOCAL_target_socket);
	noc.masterEmptyNodes[emptyMasters].LOCAL_master_init_socket.bind(noc.wrapper[wrMS].LOCAL_target_socket);
	wrMS++;
	emptyMasters++;
	noc.tableOfRouts.newEntry(0,0,MEM_SIZE);		// the third argument is the last address of the range address of LOCK device
	
       
    // NoC [0][1] -> LOCK
	noc.wrapper[wrMS].LOCAL_init_socket.bind(locker.LOCAL_target_socket);
	noc.masterEmptyNodes[emptyMasters].LOCAL_master_init_socket.bind(noc.wrapper[wrMS].LOCAL_target_socket);
	wrMS++;
	emptyMasters++;
    noc.tableOfRouts.newEntry(0,1);	


		line = 0;
		column = 2;
		
		//NoC [0][2]-> INTR_CTRL
		noc.wrapper[wrMS].LOCAL_init_socket.bind(intr_ctrl.LOCAL_target_socket);
		noc.masterEmptyNodes[emptyMasters].LOCAL_master_init_socket.bind(noc.wrapper[wrMS].LOCAL_target_socket);
		wrMS++;
		emptyMasters++;
		noc.tableOfRouts.newEntry(line,column);		// the third argument is the last address of the range address of LOCK device

		column++;

		if (r==3)
		{
			line = 1;
			column = 0;
		}

		//NoC [0][3]-> DIR
		noc.wrapper[wrMS].LOCAL_init_socket.bind(dir.LOCAL_target_socket);
		noc.masterEmptyNodes[emptyMasters].LOCAL_master_init_socket.bind(noc.wrapper[wrMS].LOCAL_target_socket);
		wrMS++;
		emptyMasters++;
		noc.tableOfRouts.newEntry(line,column);		// the third argument is the last address of the range address of LOCK device

		column++;

		if (r==4)
		{
			line = 1;
			column = 0;	
		}	


		// DFS will be binded with NoC[1]0] in case of NoC 3x3, or NoC[0][4] in other NoCs
		noc.wrapper[wrMS].LOCAL_init_socket.bind(dfs.LOCAL_target_socket);
    	noc.masterEmptyNodes[emptyMasters].LOCAL_master_init_socket.bind(noc.wrapper[wrMS].LOCAL_target_socket);
		wrMS++;
		emptyMasters++;
		noc.tableOfRouts.newEntry(line,column);	
		column++;
		if (r==5)
		{
			// in case of NoC 5x5, the next peripheral must be on node [1][0]
			line = 1;
			column = 0;
		} 
		
	//}

	// Connecting processors and noc-mesh 
	int proc = 0;


	while (line< noc.getNumberOfLines() )
        {
		while(column < noc.getNumberOfColumns())
                {
			if (proc < N_WORKERS)
			{
				
				processors[proc]->MEM_port.LOCAL_init_socket.bind(noc.wrapper[wrMS].LOCAL_target_socket);
				noc.wrapper[wrMS].LOCAL_init_socket.bind(noc.slaveEmptyNodes[emptySlaves].LOCAL_slave_target_socket);
				noc.wrapper[wrMS].initDFS(processors[proc]);

				wrMS++;
				emptySlaves++;
				noc.tableOfRouts.newEntry(line,column);
				proc++;
								
			}
			else
			{
				
				noc.wrapper[wrMS].LOCAL_init_socket.bind(noc.wrapper[wrMS].LOCAL_target_socket);
				wrMS++;
			}
			column++;
		}
		column=0;
		line++;
	}

	noc.preparingRoutingTable();
	noc.print();


	// *****************************************************************************************
	// Preparing for Simulation
    // *****************************************************************************************
	

	// Preparing the processors arguments 
	char **arguments[N_WORKERS];
	for (int i=0; i<N_WORKERS; i++){
                arguments[i] = (char **) new char*[ac];
	}
 
	for (int i=0; i<N_WORKERS; i++) {
		for (int j=0; j<ac; j++) { 
            arguments[i][j] = new char[strlen(av[j])+1];
			arguments[i][j] = av[j];
			//printf ("%s\n",arguments[i][j]);
		}
	}
	

	first_load = true;
	// Load elf before start
	for (int i=0; i<N_WORKERS; i++){	
		//printf("\ncalling load_elf to load the application %s to the memory for processor %d ",arguments[i][1],i);
		load_elf(*processors[i], mem, arguments[i][1],0x000000,MEM_SIZE);
		first_load = false;
	}

	// Setting the arguments for each processor 
	for (int i=0; i<N_WORKERS; i++){
		processors[i]->init();   // It passes the arguments to processors 
		
	}

 

	// *******************************************************************************************
	// Starting Simulation
        // *******************************************************************************************



	// Beggining of time measurement
	report_start(av[0],av[1],av[2]);

	// Beggining of simulation
    sc_start();

	// Endding the time measurement
	report_end();

	// ******************************************************************************************
	// Printing Simulation Statistics and Finishing
    // ******************************************************************************************

	// Printing statistics 
	for (int i=0; i<N_WORKERS; i++) {
		processors[i]->PrintStat(); 
	}


	// Printing statistics
	#ifdef AC_STATS
	for (i=0; i<N_WORKERS; i++){
		processors[i]->ac_sim_stats.time = sc_simulation_time(); 
		processors[i]->ac_sim_stats.print(); 
  	}
	#endif 


	#ifdef POWER_SIM
	for (int i=0; i<N_WORKERS; i++){
   		 // Connect Power Information from ArchC with PowerSC
		 processors[i]->ps.powersc_connect();
		 //processors[i]->IC.powersc_connect();
		 //processors[i]->DC.powersc_connect();
	}
	processors[N_WORKERS-1]->ps.report();
	#endif

	// Checking the status 
	bool status = 0;
	for (int i=0; i<N_WORKERS; i++)
		status = status + processors[i]->ac_exit_status; 

	noc.destroyComponents();

	for (int i=0; i<N_WORKERS; i++){
		delete processors[i];
	}
	delete processors;

	return status; 
}


void report_start(char *platform, char* application, char *cores)
{

	global_time_measures = fopen(GLOBAL_FILE_MEASURES_NAME,"a");
	local_time_measures = fopen(LOCAL_FILE_MEASURES_NAME,"a");

	printf("\nMPSoCBench: The simulator is prepared.");
	printf("\nMPSoCBench: Beggining of time simulation measurement.\n");
	gettimeofday(&startTime, NULL);
	fprintf(local_time_measures,"\n\n************************************************************************");
	fprintf(local_time_measures,"\nPlatform %s with %s cores running %s\n", platform, cores, application);
	

	fprintf(global_time_measures,"\n\n************************************************************************");
	fprintf(global_time_measures,"\nPlatform %s with %s cores running %s\n", platform, cores, application);
	
	fclose(local_time_measures);
	fclose(global_time_measures);


}
    
void report_end()
{
	global_time_measures = fopen(GLOBAL_FILE_MEASURES_NAME,"a");
	local_time_measures = fopen(LOCAL_FILE_MEASURES_NAME,"a");


	gettimeofday(&endTime, NULL);
	double tS = startTime.tv_sec*1000000 + (startTime.tv_usec);
	double tE = endTime.tv_sec*1000000  + (endTime.tv_usec);
	fprintf(local_time_measures,"\nTotal Time Taken (seconds):\t%lf", (tE-tS)/1000000);
	fprintf(global_time_measures,"\nTotal Time Taken (seconds):\t%lf", (tE-tS)/1000000);
	printf("\nTotal Time Taken (seconds):\t%lf", (tE - tS)/1000000);

	sc_core::sc_time time = sc_time_stamp();
        fprintf(local_time_measures,"\nSimulation advance (seconds):\t%lf",time.to_seconds());		
        fprintf(global_time_measures,"\nSimulation advance (seconds):\t%lf",time.to_seconds());		
        printf("\nSimulation advance (seconds):\t%lf",time.to_seconds());		

	printf("\nMPSoCBench: Ending the time simulation measurement.\n");
	fclose(local_time_measures);
	fclose(global_time_measures);
}


void load_elf(PROCESSOR_NAME &proc, tlm_memory &mem, 
              char* filename, unsigned int offset, unsigned int memsize){

  Elf32_Ehdr    ehdr;
  Elf32_Shdr    shdr;
  Elf32_Phdr    phdr;
  int           fd;
  unsigned int  i;
  //unsigned int data_mem_size=(0x4FFFFF);
  unsigned int data_mem_size=memsize;
  
 
  if (!filename || ((fd = open(filename, 0)) == -1)) 
  {
       AC_ERROR("Openning application file '" << filename << 
	             "': " << strerror(errno) << endl);
	    exit(EXIT_FAILURE);
  }
  

  //Test if it's an ELF file
  if ((read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) ||  // read header
      (strncmp((char *)ehdr.e_ident, ELFMAG, 4) != 0) ||  // test magic number
      0) {
    close(fd);
    AC_ERROR("File '" << filename << "' is not an elf. : " << strerror(errno) << endl);
    exit(EXIT_FAILURE);
  }

  //Set start address
  proc.ac_start_addr = convert_endian(4,ehdr.e_entry, proc.ac_mt_endian);
  if (proc.ac_start_addr > data_mem_size) {
    AC_ERROR("the start address of the application is beyond model memory\n");
    close(fd);
    exit(EXIT_FAILURE);
  }

  if (convert_endian(2,ehdr.e_type, proc.ac_mt_endian) == ET_EXEC)
  {
    //It is an ELF file
    if (first_load)    AC_SAY("Reading ELF application file: " << filename << endl);
    
    for (i=0; i<convert_endian(2,ehdr.e_phnum, proc.ac_mt_endian); i++)
    {
      //Get program headers and load segments
      lseek(fd, convert_endian(4,ehdr.e_phoff, proc.ac_mt_endian) + 
	    convert_endian(2,ehdr.e_phentsize, proc.ac_mt_endian) * i, SEEK_SET);
      if (read(fd, &phdr, sizeof(phdr)) != sizeof(phdr))
      {
	AC_ERROR("reading ELF program header\n");
	close(fd);
	exit(EXIT_FAILURE);
      }
  
      if (convert_endian(4,phdr.p_type, proc.ac_mt_endian) == PT_LOAD)
      {
	Elf32_Word j;
	Elf32_Addr p_vaddr = convert_endian(4,phdr.p_vaddr, proc.ac_mt_endian);
	Elf32_Word p_memsz = convert_endian(4,phdr.p_memsz, proc.ac_mt_endian);
	Elf32_Word p_filesz = convert_endian(4,phdr.p_filesz, proc.ac_mt_endian);
	Elf32_Off  p_offset = convert_endian(4,phdr.p_offset, proc.ac_mt_endian);
	//Error if segment greater then memory
	if (data_mem_size < p_vaddr + p_memsz) {
	  AC_ERROR("not enough memory in ArchC model to load application.\n");
	  close(fd);
	  exit(EXIT_FAILURE);
	}
  
	//Set heap to the end of the segment
	if (proc.ac_heap_ptr < p_vaddr + p_memsz) proc.ac_heap_ptr = p_vaddr + p_memsz;
	if(!proc.dec_cache_size)
	  proc.dec_cache_size = proc.ac_heap_ptr;
	
	//Load and correct endian
	if (first_load)
	{
		lseek(fd, p_offset, SEEK_SET);
		for (j=0; j < p_filesz; j+=sizeof(PROCESSOR_NAME_parms::ac_word)) {
		  int tmp;
		  ssize_t ret_value = read(fd, &tmp, sizeof(PROCESSOR_NAME_parms::ac_word));
		  int d=convert_endian(sizeof(PROCESSOR_NAME_parms::ac_word), tmp, proc.ac_mt_endian);
		  mem.write(p_vaddr+j+offset, 4, (unsigned char*)&tmp);
		}
	  
		int d=0;
	        for(j=p_vaddr+p_filesz;j<=p_memsz-p_filesz;j++)
		      mem.write(p_vaddr+j, 4, (unsigned char*)&d);
	}//if
	
      }//if
  
    }//for
  }//if
  
  close(fd);
}



