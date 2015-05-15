#ifdef POWER_SIM

#ifndef LOCAL_DFS_H_
#define LOCAL_DFS_H_
#include <systemc>
#include "ac_module.H"
#include "../../defines.h"

#define DELTA_T 0.0001    // 1x10^5 segundos = 100microseconds


typedef struct
{
  double low;
  double high;

}tBounds;

namespace user
{


class local_dfs:  public sc_module
{
public:

   local_dfs(sc_module_name module_name, PROCESSOR_NAME*);
  ~local_dfs();
     
   void initializePowerStates();
   int getPowerState ();
   void setPowerState (int);
   void noteFwTime();
   void noteBwTime();
   void calculateCpuUsage();
   void calculateCpuWaitTime();
   void needToDFS ();
   bool timeExceeded ();
   void readBounds ();
   
   
private:
	
	
	PROCESSOR_NAME *proc; 
  int numberOfStates;
  int *listOfStates;
  tBounds *bounds;
  double cpu_usage_rate;
  double cpu_wait_time;
  double fwTime; // last time that the wrapper insert a package into the network
  double bwTime; // last time that the wrapper return the package to the processor 
  double lastTime; // last measure of time   
  FILE* file_bounds;
};

};

#endif //LOCAL_DFS_H_

#endif