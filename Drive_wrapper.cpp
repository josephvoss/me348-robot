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
    //Ask for sensor data every 10 cm?
         float distance = l_tick*3.25;
         int multiple_100mm = floor(distance/100);
         int remainder = distance-multiple
         for(int i=0; i<multiple_100; i++)
         {
             drive(100/3.25, 100/3.25);
             cur_pos_x  += 10;
             cur_pos_y  += 10;
             //Log?
             //Set Sensor flag?
         }
         drive(remainder/3.25, remainder/3.25);
    
    }    
}  