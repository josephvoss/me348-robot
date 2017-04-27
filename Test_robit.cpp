/**
* This is the main Blank Simple C++ Project program file.
*/
#include "ping.h"
#include "abdrive.h"
#include "servo.h"

#include <stdio.h>            // Recommended over iostream for saving space
#include <propeller.h>        // Propeller-specific functions

#include "Ping_sensor.h"

/* TO DO
make robot class
  inherits the sensor class and drive class (and xbee(?) class)
  Functions start, run, end?
add IR to sensor class
  Where to put IR sensor? On the front?
make drive class
  What does this do? Just drive? Log position?

What's the goal here? Dumb robot driving around getting data?
  Maybe make dumb robot first, then smarter - event driven system?
    Requires making decisions based on data from diff cogs
  Data races? Parallel computing mess - main cog with subprocesses? What should
  be run in //?. What's the hierachy?
    Proposal: 
      1. Main cog. 
           Used for updating and controlling the system, executes childs
      2. Sensor cog.
           Reads all data at regular intervals, uses lots of waits.
              How much memory to alloc to? Tbh I think only need 3 reads per cycle
              front left and right.
      3. Drive cog.
           Drives and tracks current position. Set data storage to sensor cog/3, 
           stores data at the same clock cycle of dead reckoning with sensor cog
               Should this be done separately from the drive function?
               Maybe use wrapper for abdrive that tracks position with clock.
               How to syncronize with sensor?
                   Just assume current drive data goes with current sensor data?
      4. Communication cog.
           Nothing to it. Takes data from sensor and drive and sends to labview.
      5. Decision cog.
           Takes current data. Switch to see if this requires a decision.
           If so, count options. If 1, do it. If >1, log amount of options, pos 
           and do first 1. If 0, go to last option pos >1, decrement, continue.
           Overwrite array with new thing.
               Mem needed: give space for 15 options. Each option has option count
               and position (x,y)
               
       Oh shit that was a lot. Complicated stuff will be comm and decision. These 
       cogs can be merged for simplicity if needed.
       
       Est timeline
       1. Sensor cog and logging method
           How is mem shared? Local or shared?
       2. Drive cog and pos tracking
       3. Decision
       4. Comm
*/

void init_all()
{
/* Benito Suggest
	Init, then wait to start until sonar reads 1cm, once hand moves start
	Also search for walls 
*/
  pause(2000);
  printf("Initialized Successfully\n");
}

void exit_all()
{
//  delete turret;
  printf("Exited Successfully\n");
}  

int main(){
  init_all();
  Ping_sensor turret(16, 19, 5);
  while (1 == 1)
  {
    turret.read();
    for (int i=0; i<5; i++)
    {
      printf("%d\n", turret.distance_arr[i]); 
    }
    printf("\n");
    pause(5000);
  }   
  return 0;
}

