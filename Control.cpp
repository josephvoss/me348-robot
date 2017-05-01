#include "Control.h"

Control::Control(Ping_sensor* ping_in, Drive_wrapper* drive_in)
{
    //Init pointers to the add of input classes
    ping = ping_in;
    drive = drive_in;

    drive_flag = malloc(sizeof(int));
    decide_arr = malloc(sizeof(Decide_tuple)*10);
    decide_count = 0;
}

Control::~Control(void)
{
    free(drive_flag);
    free(decide_arr);
}

void Control::decide(int left, int right, int straight)
/*
 * Either turns left, right, or straight, and logs that decision with the
 * current position into the decision array
 */
{
    //Make decision tuple
    Decide_tuple current;
    current.pos_x = driver->get_pos_x(); 
    current.pos_y = driver->get_pos_y(); 
    // Assumes only option is to move forward
    current.l_flag = 0; current.r_flag = 0; current.s_flag = 1; int options = 1;

    if (left > 10) { options++; current.l_flag = 1; }
    if (right > 10) { options++; current.r_flag = 1; }
    if (straight < 15) { options--; current.s_flag = 0; }

    if (options == 1)
    {
        if (current.l_flag) { driver->turn_left(); }
        if (current.r_flag) { driver->turn_right(); }
        //Should never trigger here, 
        //if straight was only option should have continued
        if (current.s_flag) { printf("Error!\n"); }
    }

    //Decide on direction
    if (options > 1)
    {
        dir = rand() % options;
        if (current.l_flag && current.r_flag && current.s_flag)
        {
            if (dir == 1) { driver->turn_left(); }
            if (dir == 2) { driver->turn_right(); }
//          if (dir == 3) { driver->go_straight(); } 
        }
        else if (current.l_flag && current.r_flag)
        {
            if (dir == 1) { driver->turn_left(); }
            if (dir == 2) { driver->turn_right(); }
        }
        else if (current.l_flag && current.s_flag)
        {
            if (dir == 1) { driver->turn_left(); }
//          if (dir == 2) { driver->go_straight); }
        }
        else if (current.r_flag && current.s_flag)
        {
            if (dir == 1) { driver->turn_left(); }
//          if (dir == 2) { driver->go_straight); }
        }
    }

    if (options == 0) 
    {
       if (decide_count == 0) 
           printf("Error! Should have 1 decision made before gets stuck\n");
       else
       {
           //Turn around and go to last decision point, take road not travelled
           driver->turn_left();
           driver->turn_right();
           //Decide_tuple old = decide_arr[decide_count - 1];
           
           //TO BE ADDED
       }

    }

    //Log decision
    decide_arr[decide_count] = current;
    decide_arr[decide_count].pos_x = driver->get_pos_x(); 
    decide_arr[decide_count].pos_y = driver->get_pos_y(); 

}

void Control::main(void)
{
    //Pointer to data address being updated by sensor class
    int** sensor_data = &(ping_in->distance_arr);

    cog_run(ping_in->read());

    //Pass drive_flag address to drive function
    *drive_flag = 1;
    cog_run(driver->drive(25, drive_flag));

    while (1)
    {
        //Loop over sensor data
        for (int i = 0; i < 5; i++)
        {
            // Case in which to stop moving
            //      Wall in front
            //      Line in front
            //      Space to the left
            //      Space to the right
            if ((*sensor_data)[i] < 15)
            {
                drive_flag = 0;
            }
        }
        //Example for now, sensor_data array format unspecified
        left = (*sensor_data)[0];
        right = (*sensor_data)[2];
        straight = (*sensor_data)[1];
        decide(left, right, straight); // Turn either left, right, or neither
        drive_flag = 1
    }
};
