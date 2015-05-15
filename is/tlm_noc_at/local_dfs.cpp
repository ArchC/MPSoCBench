
#ifdef POWER_SIM

#include "local_dfs.h"
#include <tlm.h>

#define measures 1

using user::local_dfs;

/*uint32_t _swap(uint32_t value)
{
  #ifdef AC_GUEST_BIG_ENDIAN
    return be32toh(value);
  #else
    return le32toh(value);
  #endif
}
*/

local_dfs::local_dfs( sc_module_name module_name, PROCESSOR_NAME* proc ):
sc_module( module_name )
{
  this->proc = proc;

  fwTime = 0;
  bwTime = 0;
  cpu_wait_time = 0;
  cpu_usage_rate = 0;

  initializePowerStates();
}

void local_dfs::noteFwTime()
{
  fwTime = sc_time_stamp().to_seconds();
}

void local_dfs::noteBwTime()
{
  bwTime = sc_time_stamp().to_seconds();
  calculateCpuWaitTime();
  if (timeExceeded()) calculateCpuUsage();
  needToDFS ();
}

void local_dfs::needToDFS ()
{
    int actual_state = proc->ps.getPowerState();
    int state = 0;

    for (int i=0; i<numberOfStates; i++)
    {
      if ((cpu_usage_rate >= bounds[i].low) && (cpu_usage_rate < bounds[i].high))
        state = i;
    }
    
    if (state != actual_state) setPowerState(state);

}
bool local_dfs::timeExceeded ()
{
  if (bwTime >= lastTime+DELTA_T)
  {
    lastTime = bwTime;
    return true;
  }
  return false;

}
void local_dfs::calculateCpuWaitTime()
{
  cpu_wait_time += (bwTime - fwTime);
}

void local_dfs::calculateCpuUsage()
{
  
    double final_time = sc_time_stamp().to_seconds();
    cpu_usage_rate = (final_time-cpu_wait_time)/final_time;
    
  
}

local_dfs::~local_dfs()
{
  calculateCpuUsage();
  printf("\nProc%d usage rate: %.10lf\% \n", proc->getId(), cpu_usage_rate*100);
  delete [] listOfStates;
}

void local_dfs::readBounds ()
{
  file_bounds = fopen ("bounds.txt","r");
  if (!file_bounds)
  {
      printf("\nError opening file in local_dfs.");
      exit(1);
  }

  double x;
  for (int i=0; i<numberOfStates; i++)
  {

    fscanf(file_bounds,"%lf", &x);
    bounds[i].low = x;
    printf("\nThe low bound for state %d is %.1lf", i, bounds[i].low);
    fscanf(file_bounds,"%lf", &x);
    bounds[i].high = x;
    printf("\nThe high bound for state %d is %.1lf", i, bounds[i].high);
  }

}

void local_dfs::initializePowerStates()
{

   
  if (DFS_DEBUG) printf("\nInitializing LOCAL_DFS");

  numberOfStates = (proc->ps).getNumberOfStates();
  listOfStates = new int [numberOfStates];
  bounds = new tBounds [numberOfStates];

  (proc->ps).completeListOfStates(listOfStates); 
  readBounds (); 
  
  if (DFS_DEBUG) 
  {
    printf("\nLOCAL_DFS: There are %d available frequencies for Processor %d: ", numberOfStates, proc->getId() );
    
    for (int j=0; j<numberOfStates; j++)
    {
      printf("\nState %d , Bounds-> %f,%f",listOfStates[j], bounds[j].low, bounds[j].high);
    }    
  }

  if (DFS_DEBUG) printf("\nLOCAL_DFS: Initializing Processor %d with power state %d.",proc->getId(),0);
  setPowerState (0);
      
  }




// POWER CONTROL
int local_dfs::getPowerState ()
{

    if (DFS_DEBUG) printf("\nDFS: getPowerState is returning the power state of processor %d", proc->getId());
    return (proc->ps).getPowerState();

}

void local_dfs::setPowerState (int state)
{

  
   int newFrequency;
   if ( (state >= 0) && (state < numberOfStates) )
   {
        
      if (DFS_DEBUG)
      {
        printf("\nLOCAL_DFS: setPowerState is updating the power state of processor %d", proc->getId());
        printf("\nLOCAL_DFS: setPowerState -> old power state:%d\tnew power state->%d", getPowerState(), state);
      }

        newFrequency = listOfStates[state];
        proc->set_proc_freq(newFrequency);
        if (DFS_DEBUG)
        { 
          printf("\nLOCAL_DFS: Updating processor frequency->%d",newFrequency);
        }
   }
   else
   { 
        printf("\nLOCAL_DFS: The new frequency required is no availabe for this processors;");
        printf("\nLOCAL_DFS: Maintaining the current frequency.");
   }    

   (proc->ps).setPowerState(state);
}

#endif