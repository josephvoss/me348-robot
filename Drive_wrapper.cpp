#include "Drive_wrapper.h"

#include "abdrive.h"

Drive_wrapper::Drive_wrapper(int pin_l, int pin_r, int pin_le, int pin_re, int log_length)
{
    
    //Init array length
    pos_arr_length = log_length;
    
    //Init position_array, 2 dimensional array of size [log_length,2]
    position_array = (int**) malloc(log_length*sizeof(int*));
    for (int i=0; i<log_length; i++)
    {
        position_array[i] = (int*) malloc(2*sizeof(int));
    }      

    //Set initial position
    cur_pos_x = 0;
    cur_pos_y = 0;

    orientation = 0;
    
    //Init drive system
    drive_servoPins(pin_l,pin_r);
    drive_encoderPins(pin_le, pin_re);
       
}

Drive_wrapper::~Drive_wrapper()
{    
    for (int i=0; i<pos_arr_length; i++)
    {
        free(position_array[i]);
    }
    free(position_array);
}

void Drive_wrapper::drive(int speed, int* flag_drive)
{
    //Ask for sensor data every 15 ticks?
    int x = 0; int y = 0; int l_count; int r_count;
    while(1)
    {    
        while (*drive_flag)
        {
            drive_speed(speed, speed); // blocking
            
            //Log?
            //Set Sensor flag?
        }
    }
    drive_speed(0,0);
    drive_getTicks(&l_count, &r_count); //Calc actual no. travelled
    
    //Logic to determine direction
    //   0
    // 3   1
    //   2
    
    //ugly code block but at least it's reasonable
    x = 0; y = 0;
    if (orientation == 0) y = 1;
    if (orientation == 1) x = 1;
    if (orientation == 2) y = -1;
    if (orientation == 3) x = -1;
    cur_pos_y += l_count*y;
    cur_pos_x += r_count*x;
}

void Drive_wrapper::turn(int turnLeft, int turnRight)
{
    int l_count_d; int r_count_d; int l_count_e; int r_count_e;
    
    //Logic to take care of count amounts to turn each wheel
    // and to update the orientation of the bot
    if (turnLeft) { l_count_d = -25; r_count_d = 26;
                    orientation = (orientation - 1) % 4;}
    if (turnRight) { l_count_d = 26; r_count_d = -25;
                     orientation = (orientation + 1) % 4;}
    
    //90 deg turn
    drive_goto(l_count_d,r_count_d);
    
    //Overshoot protection (But only protects from 1?
    drive_getTicks(&l_count_e, &r_count_e); //Calc actual no. travelled
    l_count_d = l_count_d - l_count_e;
    r_count_d = r_count_d - r_count_e;
    drive_goto(l_count_d,r_count_d);
    
}  
