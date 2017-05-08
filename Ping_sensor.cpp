#include <stdio.h>            // Recommended over iostream for saving space
#include <propeller.h>        // Propeller-specific functions

#include "math.h"
#include "simpletools.h"                      // Include simple tools

#include "Ping_sensor.h"

Ping_sensor::~Ping_sensor(void)
{
  printf("Deleting Ping_sensor\n");
  free(angle_arr);
//  free(distance_arr); //Not using data array, just singular reads
}

Ping_sensor::Ping_sensor(int pin_s, int pin_p, int pin_lir, int pin_rir, int pin_lqt, int pin_rqt)
{
    // Init all sensors
    pin_servo = pin_s;
    pin_ping = pin_p;
    pin_left_ir = pin_lir;
    pin_right_ir = pin_rir;
    pin_left_qt = pin_lqt;
    pin_right_qt = pin_rqt;

//    count = read_count;
    //Init angle_arr with values to read
    int num_pts = 3;
    angle_arr = (int*) malloc(num_pts*sizeof(int));
    int i=0;
    int angle_diff = 180/(num_pts-1);
    for (i=0; i<num_pts; i++)
    {
     angle_arr[i] = i*angle_diff*10;
    }

   //Init distance_array
//   distance_arr = (Sensor_data*) malloc(read_count*sizeof(Sensor_data));
}

void Ping_sensor::run(void)
{
  //Code to be run by cog. Basically call read every second
  while (1)
  {
      while(*flag)
      {
          this->read();
          pause(500);
      }
  }
}

void Ping_sensor::read(void)
{
    Sensor_data new_read;
    servo_angle(pin_servo, 0);
    pause(500);
    int num_pts = 3;
    for (int x=0; x<num_pts; x++)
    {
      servo_angle(pin_servo, angle_arr[x]);
      int num_avg = 5;
      int ping_dist = 0;
      pause(1000);
      for (int i=0; i<num_avg; i++)
      {
        ping_dist += ping_cm(pin_ping);
      }
      
      new_read.ping[x] = ping_dist/num_avg;
    }

    // Add QTI and IR reads here

    // Adds current read to data array
      data = new_read;
//    distance_arr[count] = new_read;
//    count++;
}
