#include "Drive_wrapper.h"

#include "abdrive.h"

Drive_wrapper::Drive_wrapper(int pin_l, int pin_r, int log_length)
{
    //Init Pins
    pin_left = pin_l;
    pin_right = pin_r;
    
    //Init array length
    pos_arr_length = log_length;
    
    //Init position_array, 2 dimensional array of size [log_length,2]
    position_array = (int**) malloc(log_length*sizeof(int*));
    for (int i=0; i<log_length; i++)
    {
        positon_array[i] = (int*) malloc(2*sizeof(int));
    }      

    //Set initial position
    curr_pos_x = 0;
    curr_pos_y = 0;

    orientation = 0;    
}

Drive_wrapper::~Drive_wrapper()
{    
    for (int i=0; i<pos_arr_length; i++)
    {
        free(position_array[i]);
    }
    free(position_array);
}

void Drive_wrapper::drive(int l_tick, int r_tick)
{
    //if straight
    if (l_tick == r_tick)
    {
    //Ask for sensor data every 30 ticks?
         int distance = l_tick;
         int multiple_ticks = floor(distance/30);
         int remainder = distance-multiple_ticks;
         for(int i=0; i<multiple_100; i++)
         {
             drive(multiple_ticks, multiple_ticks);
             
             //Logic to determine direction
             //   0
             // 3   1
             //   2
             
             //ugly code block but at least it's reasonable
             int x = 0; int y = 0;
             if (orientation == 0) y = 1;
             if (orientation == 1) x = 1;
             if (orientation == 2) y = -1;
             if (orientation == 3) x = -1;
             cur_pos_y  += 10*y
             cur_pos_x  += 10*x
             
             //Log?
             //Set Sensor flag?
         }
         drive(remainder, remainder);
    
    }    
}  