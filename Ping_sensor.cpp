#include <stdio.h>            // Recommended over iostream for saving space
#include <propeller.h>        // Propeller-specific functions

#include "math.h"
#include "simpletools.h"                      // Include simple tools

#include "Ping_sensor.h"

Ping_sensor::~Ping_sensor(void)
{
  printf("Deleting Ping_sensor\n");
  free(angle_arr);
  free(distance_arr); 
}

Ping_sensor::Ping_sensor(int pin_s, int pin_p, int read_count)
{
   pin_servo = pin_s;
   pin_ping = pin_p;
   count = read_count;
   //Init angle_arr with values to read
   angle_arr = (int*) malloc(read_count*sizeof(int));
   int i=0;
   int angle_diff = 180/(read_count-1);
   for (i=0; i<read_count; i++)
   {
    angle_arr[i] = i*angle_diff*10;
    printf("angle: %d\n", angle_arr[i]);
   }
   //Init distance_array
   distance_arr = (int*) malloc(read_count*sizeof(int));
}

void Ping_sensor::run(void)
{
  //Code to be run by cog. Basically call read at regular intervals
  
  //Bad way to do it, but could we time the read loop, then have it wait
  //like 5 sec, then read again? Shitty way to sync with other cogs, but 
  //idk a better way
  this->run();
  pause(5000);
}  

void Ping_sensor::read(void)
{
    servo_angle(pin_servo, 0);
    pause(200);
    for (int i=0; i<count; i++)
    {
      servo_angle(pin_servo, angle_arr[i]);
      pause(200);
      distance_arr[i] = ping_cm(pin_ping);
      pause(200); //wait 1th of a second
    }    
} 
