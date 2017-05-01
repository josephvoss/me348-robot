#include "Control.h"

Control::Control(Ping_sensor* ping_in, Drive_wrapper* drive_in)
{
    //Init pointers to the add of input classes
    ping = ping_in;
    drive = drive_in;

    drive_flag = (int*) malloc(sizeof(int));
    sense_flag = (int*) malloc(sizeof(int));
    decide_arr = (Decide_tuple*) malloc(sizeof(Decide_tuple)*10);
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
    Sensor_Data* sensor_data = &(ping_in->data;

    //Pass sense_flag address to sense function
    *sense_flag = 1;
    cog_run(ping_in->run(sense_flag));

    //Pass drive_flag address to drive function
    *drive_flag = 1;
    cog_run(driver->drive(25, drive_flag));

    int dist_r; int dist_l; int dist_s;
    while (1)
    {

        // Case in which to stop moving
        //      Wall in front
        //      Line in front
        //      Space to the left
        //      Space to the right
        

        dist_l = sensor_data.ping[0];
        dist_s = sensor_data.ping[1];
        dist_r = sensor_data.ping[2];
        if (dist_s < 15 || dist_r < 10 || dist_l < 10)
        {
            drive_flag = 0;
            sense_flag = 0;
        }

        //Example for now, sensor_data array format unspecified
        left = dist_l;
        right = dist_r;
        straight = dist_s;
        decide(left, right, straight); // Turn either left, right, or neither
        drive_flag = 1;
        pause(1000);
        sense_flag = 1;
    }
}
