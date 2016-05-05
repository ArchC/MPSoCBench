#ifdef POWER_SIM


#include "local_dfs.h"
#include <tlm.h>

#define measures 1

using user::local_dfs;


local_dfs::local_dfs(PROCESSOR_NAME* proc, sc_module_name module_name ):
sc_module( module_name )
{
  
  this->proc   = proc;
  this->numberOfStates = 0;
  this->listOfStates = NULL;  

  #ifdef  DFS_AUTO_SELECTION_CPU_RATE
  this->fwTime = 0;
  this->bwTime = 0;
  this->bounds = NULL;
  this->deltaCounter = 0;
  this->cpu_wait_time = 0;
  this->cpu_usage_rate = 0;
  this->max_cpu_usage_rate = 0;
  this->min_cpu_usage_rate = 1;
  #endif 

  #ifdef  DFS_AUTO_SELECTION_ENERGY_STAMP
  this->lastTimeES = 0;
  this->end_of_elab_phase = false;
  this->end_of_init_phase = false;
  energyStamp = NULL;
  #endif

  initializePowerStates();
}



local_dfs::local_dfs(PROCESSOR_NAME* proc):
module_name("dfs"),
sc_module((sc_module_name)module_name)
{
  
  this->proc   = proc;
  this->numberOfStates = 0;
  this->listOfStates = NULL;  

  #ifdef  DFS_AUTO_SELECTION_CPU_RATE
  this->fwTime = 0;
  this->bwTime = 0;
  this->bounds = NULL;
  this->deltaCounter = 0;
  this->cpu_wait_time = 0;
  this->cpu_usage_rate = 0;
  this->max_cpu_usage_rate = 0;
  this->min_cpu_usage_rate = 1;
  #endif 

  #ifdef  DFS_AUTO_SELECTION_ENERGY_STAMP
  this->lastTimeES = 0;
  this->end_of_elab_phase = false;
  this->end_of_init_phase = false;
  energyStamp = NULL;
  #endif

  initializePowerStates();
}

local_dfs::~local_dfs()
{
  
  #ifdef DFS_AUTO_SELECTION_CPU_RATE
  calculateCpuUsage();
  if (DFS_DEBUG) printf("\nProc%d usage rate: %.3lf\n", proc->getId(), cpu_usage_rate*100);
  delete [] bounds;
  #endif

  #ifdef DFS_AUTO_SELECTION_ENERGY_STAMP
  delete [] energyStamp;
  #endif

  delete [] listOfStates;
}




void local_dfs::initializePowerStates()
{

  if (DFS_DEBUG) printf("\nInitializing LOCAL_DFS");

  numberOfStates = (proc->ps).getNumberOfStates();
  listOfStates = new int [numberOfStates];
  (proc->ps).completeListOfStates(listOfStates); 
  
  if (DFS_DEBUG) printf("\nLOCAL_DFS: There are %d available frequencies for Processor %d. ", numberOfStates, proc->getId() );

  #ifdef DFS_AUTO_SELECTION_CPU_RATE
  bounds = new tBounds [numberOfStates];
  readBounds (); 
  if (DFS_DEBUG) 
  {
    for (int j=0; j<numberOfStates; j++)
    {
      printf("\nState %d , Bounds-> %f,%f",listOfStates[j], bounds[j].low, bounds[j].high);
    }    
  }
  #endif

  #ifdef DFS_AUTO_SELECTION_ENERGY_STAMP
  energyStamp  = new double [numberOfStates];
  
  for (int i=0; i<numberOfStates; i++)   energyStamp[i] = 0;
  #endif

  setInitialPowerState (INITIAL_PW_STATE);

  //if (proc->getId() >= 0 && proc->getId() < 16)  setInitialPowerState (3);
  //else if (proc->getId() >= 8 && proc->getId() < 16) setInitialPowerState (3);  
  //else if (proc->getId() >= 8 && proc->getId() < 12) setInitialPowerState (2);  
  //else if (proc->getId() >= 12 && proc->getId() < 16) setInitialPowerState (3);  
  

  
  }


void local_dfs::setInitialPowerState(int state)
{
  if (DFS_DEBUG) printf("\nLOCAL_DFS: Initializing Processor %d with power state %d.",proc->getId(),state);
  int newFrequency = listOfStates[state];
  proc->set_proc_freq(newFrequency);
  (proc->ps).setPowerState(state);
}

// POWER CONTROL
int local_dfs::getPowerState ()
{
  return (proc->ps).getPowerState();
}


void local_dfs::setPowerState (int state)
{

   if ( (state >= 0) && (state < numberOfStates) )
   {

      int state_cur = getPowerState();

      if (state_cur != state)
      {  
          int newFrequency = listOfStates[state];
          proc->set_proc_freq(newFrequency);
          (proc->ps).setPowerState(state);

          double t = sc_time_stamp().to_seconds();

          //printf("\n%.8f,%d,%d,%d", t, proc->getId(), state_cur, state);
          if (DFS_DEBUG)
          {
            //printf("\nLOCAL_DFS: cpu_usage_rate of processor %d is %0.4lf", proc->getId(),cpu_usage_rate*100);
            printf("\nLOCAL_DFS: updating the power state of processor %d", proc->getId());
            printf("\nLOCAL_DFS: old power state:%d\tnew power state->%d", state_cur, state);
            printf("\nLOCAL_DFS: Updating processor frequency->%d",newFrequency);
          }

      }
   }
   else
   { 
        printf("\nLOCAL_DFS: The new frequency required is not availabe for this processors;");
        printf("\nLOCAL_DFS: Maintaining the current frequency.");
   }    

}


#ifdef  DFS_AUTO_SELECTION_ENERGY_STAMP

void local_dfs::autoSelectionEnergyStamp ()
{
  double t = sc_time_stamp().to_seconds();
  
 
  if (!end_of_elab_phase)
  {  
    //if (t >= (lastTimeES + FIRST_DELTA_T + DELTA_T))
    if (t >= (lastTimeES + DELTA_T))
    {
      //printf("\nFase de elaboração: nova avaliação");
      int actualState = getPowerState();
      
      energyStamp[actualState] = (proc->ps).get_energy_stamp(actualState);       
      //printf("\nenergyStamp[%d]: %.20lf", actualState, energyStamp[actualState]);

      if (DFS_DEBUG)
      {
        printf("\n\n\n----------------");
        printf("\nProcessor %d Elaboration phase:", proc->getId());
        printf("\nActual State: %d", actualState);
        printf("\nDelta: %0.5lf", DELTA_T);
        printf("\nLast time measured: %0.5lf", t);
        printf("\nEnergyStamp:%.10lf \n", energyStamp[actualState]);
      }
     
      actualState++;
      
      if (actualState == numberOfStates)
      {
          end_of_elab_phase = true;
          actualState = getBestFrequency (); 

        //  printf("\nAcabou fase de elaboração");
        //  printf("\nBest state for processor %d is %d", proc->getId(), actualState);
      }
      
      setPowerState(actualState);
      (proc->ps).initialize_energy_stamp();

      lastTimeES = t; /*****/
    }
  }
  else // the elaboration phase is over
  {
      //if (t >= lastTimeES + FIRST_DELTA_T + DELTA_T)
      if (t >= lastTimeES + DELTA_T)
      {

        int actualState = getPowerState();
      
        //double ES_cur = (proc->ps).get_energy_stamp(actualState);

        double NewES_cur = (proc->ps).get_energy_stamp(actualState);
        energyStamp[actualState] = NewES_cur;

       // printf("\nNewEScur: %.20lf", NewES_cur);

        int newState = actualState;

        for (int i=0; i<numberOfStates; i++)
        {
            //if (ES_cur < energyStamp[i]) newState = i;
            if (NewES_cur < energyStamp[i]) newState = i;

        }
        if (newState != actualState)
        {
            setPowerState (newState);
 
        } 
        (proc->ps).initialize_energy_stamp();
        lastTimeES = t;
      }  

  } 
}

int local_dfs::getBestFrequency ()
{
    int bestState = 0;
    
    double newMinES = energyStamp[0];
    
    for (int i=0; i<numberOfStates; i++)
    {
      
      printf("\nProcessor %d: energyStamp[%d] = %.20lf", proc->getId(), i, energyStamp[i]);
      if (energyStamp[i] < newMinES)
      {
        bestState = i;
      }
    }
    return bestState;

} 
#endif


#ifdef  DFS_AUTO_SELECTION_CPU_RATE
void local_dfs::noteFwTime()
{
  fwTime = sc_time_stamp().to_seconds();
}

void local_dfs::noteBwTime()
{
  bwTime = sc_time_stamp().to_seconds();

}

void local_dfs::autoSelectionCPUrate()
{
  calculateCpuWaitTime();
  if (timeExceeded()) 
  {
    calculateCpuUsage();
    needToDFS ();
    deltaCounter ++;
  }

  if (deltaCounter > N_DELTA_T)
  {
    remakeBounds();
  }
}

void local_dfs::remakeBounds()
{
  
  if (DFS_DEBUG)
  {
    printf("\nProcessor %d remaking bounds.", proc->getId() );
    printf("\nDelta Counter: %d", deltaCounter);

    printf("\n\nBefore Remake Bounds:");

    for (int i=0; i<numberOfStates; i++)
    {
      printf("\nMin_cpu_usage_rate: %.3f", min_cpu_usage_rate*100);
      printf("\nMax_cpu_usage_rate: %.3f", max_cpu_usage_rate*100);
      printf("\nThe low bound for state %d is %.1f", i, bounds[i].low);
      printf("\nThe high bound for state %d is %.1f", i, bounds[i].high);
    }
  }

  deltaCounter = 0;


  bounds[0].low  = 0;
  bounds[0].high = min_cpu_usage_rate*100/2;


  bounds[1].low  = bounds[0].high - 0.25*(bounds[0].high);
  bounds[1].high = min_cpu_usage_rate*100;


  bounds[2].low  = bounds[1].high - 0.25*(bounds[1].high);
  bounds[2].high = ((max_cpu_usage_rate+min_cpu_usage_rate)/2)*100;


  bounds[3].low  = bounds[2].high - 0.25*(bounds[2].high);
  bounds[3].high = 100;


  min_cpu_usage_rate = 1;
  max_cpu_usage_rate = 0;

  if (DFS_DEBUG)
  {
    printf("\n\nAfter Remake Bounds:");

    for (int i=0; i<numberOfStates; i++)
    {
      printf("\nThe low bound for state %d is %.1f", i, bounds[i].low);
      printf("\nThe high bound for state %d is %.1f", i, bounds[i].high);
    }
  }
}

void local_dfs::needToDFS ()
{
    int actual_state = proc->ps.getPowerState();
    
    // is the cpu usage rate is not between the low and the high bounds for this state
    // we need to find the best state for this threshold

    if (! ((cpu_usage_rate*100 > bounds[actual_state].low)&&(cpu_usage_rate*100 <= bounds[actual_state].high)))
    {
      for (int i=0; i<numberOfStates; i++)
      {
        if ((cpu_usage_rate*100 > bounds[i].low) && (cpu_usage_rate*100 <= bounds[i].high))
        {  
          setPowerState(i);
          break;
        } 
      }
    }

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
    //printf("\n\nProc %d", proc->getId());
    //printf("\ncpu_wait_time: %lf", cpu_wait_time);
    //printf("\ncpu_rate: %lf",cpu_usage_rate*100);
    //printf("\nmin_cpu_rate: %lf",min_cpu_usage_rate*100);
    //printf("\nmax_cpu_rate: %lf",max_cpu_usage_rate*100);

    if (cpu_usage_rate > max_cpu_usage_rate) max_cpu_usage_rate = cpu_usage_rate;
    if (cpu_usage_rate < min_cpu_usage_rate) min_cpu_usage_rate = cpu_usage_rate;
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
  int temp;
  for (int i=0; i<numberOfStates; i++)
  {
    temp = fscanf(file_bounds,"%lf", &x);
    bounds[i].low = x;
    
    temp = fscanf(file_bounds,"%lf", &x);
    bounds[i].high = x;
    if (DFS_DEBUG)
    {
      printf("\nThe low bound for state %d is %.1f", i, bounds[i].low);
      printf("\nThe high bound for state %d is %.1f", i, bounds[i].high);
    } 
  }

}


#endif
#endif
