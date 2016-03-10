#ifdef POWER_SIM

#ifndef LOCAL_DFS_H_
#define LOCAL_DFS_H_
#include <systemc>
#include "ac_module.H"
#include "../../defines.h"

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




   local_dfs(PROCESSOR_NAME*, sc_module_name module_name);
   local_dfs(PROCESSOR_NAME*);


  ~local_dfs();
     
   void initializePowerStates();
   int  getPowerState ();
   void setPowerState (int);
   void setInitialPowerState (int state);

   #ifdef  DFS_AUTO_SELECTION_CPU_RATE
   void noteFwTime();
   void noteBwTime();
   void calculateCpuUsage();
   void calculateCpuWaitTime();
   void readBounds ();
   void remakeBounds();
   void autoSelectionCPUrate();
   void needToDFS ();
   bool timeExceeded ();
   #endif

   #ifdef  DFS_AUTO_SELECTION_ENERGY_STAMP
   void autoSelectionEnergyStamp();
   int  getBestFrequency (); 

   #endif

private:
  
  PROCESSOR_NAME *proc;
  int     numberOfStates;
  int     *listOfStates;
  
  #ifdef  DFS_AUTO_SELECTION_CPU_RATE
  double  cpu_usage_rate;
  double  max_cpu_usage_rate;
  double  min_cpu_usage_rate;
  double  cpu_wait_time;
  double  fwTime;   // last time that the wrapper insert a package into the network
  double  bwTime;   // last time that the wrapper return the package to the processor 
  double  lastTime; // last measure of time
  int     deltaCounter; 
  tBounds *bounds;
  FILE*   file_bounds;
  #endif

  
  const char *module_name;

  #ifdef  DFS_AUTO_SELECTION_ENERGY_STAMP
  double  lastTimeES;
  double  *energyStamp;
  bool    end_of_elab_phase;
  bool    end_of_init_phase;
  #endif
  

};

};




#endif //LOCAL_DFS_H_

#endif