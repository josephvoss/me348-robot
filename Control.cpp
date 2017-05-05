#include "Control.h"
#include "Ping_sensor.h"

#include <propeller.h>
#include <stdio.h>
#include <abdrive.h>

Control::Control(Ping_sensor* ping_in, Drive_wrapper* drive_in)
{
    //Init pointers to the add of input classes
    ping = ping_in;
    driver = drive_in;

    drive_flag = (int*) malloc(sizeof(int));
    sense_flag = (int*) malloc(sizeof(int));
    decide_arr = (Decide_tuple*) malloc(sizeof(Decide_tuple)*10);
    decide_count = 0;
    
    ping->set_flag(sense_flag);
    driver->set_flag(drive_flag);
}

Control::~Control(void)
{
    free(drive_flag);
    free(decide_arr);
}

void Control::decide(int* cardinal_arr)
/*
 * Either turns left, right, or straight, and logs that decision with the
 * current position into the decision array
 */
{
    //Make decision tuple
    Decide_tuple current;
    current.pos_x = driver->get_pos_x(); 
    current.pos_y = driver->get_pos_y();
    int orientation = driver->get_orientation();
    
    printf("Orientation: %d\n", orientation);
    for (int i = 0; i < 4; i++)
        printf("%d ", cardinal_arr[i]);
    printf("\n");
    
    // Assume only option is forward
    current.dir_arr[0] = 0;
    int options = 0;
    for (int i = 0; i < 4; i++)
    {
        //If straight
        if (i == orientation && cardinal_arr[i] > 15)
            { options++; current.dir_arr[i] = 1; }
        
        //If right
        if (i == orientation + 1 && cardinal_arr[i] > 15)
            { options++; current.dir_arr[i] = 1; }
        
        //If left
        if (i == orientation - 1 && cardinal_arr[i] > 15)
           {  options++; current.dir_arr[i] = 1; }
    }      

    printf("No. of options available: %d\n",options);

    //Find only direction avail
    if (options == 1)
    {
        for (int i = 0; i++; i< 4)
        {
            if (current.dir_arr[i] && i == orientation - 1)
                { driver->turn_left(); break; }
            if (current.dir_arr[i] && i == orientation + 1)
                { driver->turn_right(); break; }
                
            //Should never trigger here, 
            //if straight was only option should have continued                
            if (current.dir_arr[i] && i == orientation)
                printf("Error!\n");
        }        
    }

    //Decide on direction
    if (options > 1)
    {
        int dir = rand() % options;
        int i;
        for (int x = 0; x < 4; x++)
        {
            i = (x+dir) % 4;
            if (current.dir_arr[i] && i == orientation - 1)
                { driver->turn_left(); break; }
            if (current.dir_arr[i] && i == orientation + 1)
                { driver->turn_right(); break; }       
            if (current.dir_arr[i] && i == orientation)
                { break; }
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
           
           Decide_tuple old = decide_arr[decide_count - 1];
           int delx = current.pos_x - old.pos_x;
           int dely = current.pos_y - old.pos_y;
           if (delx != 0) drive_goto(delx, delx);
           if (dely != 0) drive_goto(dely, dely);
           
           //Setup new sensor data array, set positions able to travel with distance 
           //of 20cm free
           int* new_cardinal_arr = (int*) old.dir_arr;
           for (int i = 0; i<4; i++)
              new_cardinal_arr[i] *= 20;
           
           //Set route taken previously sensor readings to <10
           //artificially blocks route?
           if (delx < 0) new_cardinal_arr[3] = 5;
           if (delx > 0) new_cardinal_arr[1] = 5;
           if (dely < 0) new_cardinal_arr[0] = 5;
           if (dely > 0) new_cardinal_arr[2] = 5;
           
           //Call function recursively
           decide( (int*) new_cardinal_arr);
           
       }
    }

    //Log decision
    decide_arr[decide_count] = current;
}

void Control::main(void)
{
    //Pointer to data address being updated by sensor class
    Sensor_data* sensor_data = &(ping->data);

    //Pass sense_flag address to sense function
    *sense_flag = 1;

    //Pass drive_flag address to drive function
    *drive_flag = 1;

    int dist_r=20; int dist_l=20; int dist_s=20;
    while (1)
    {
        // Cases in which to stop moving
        //      Wall in front
        //      Line in front
        //      Space to the left
        //      Space to the right
        
        //Start drive
        int x = 0; int y = 0; int l_count; int r_count;

        driver->drive(10);
        ping->read();        
        dist_r = sensor_data->ping[0];
        dist_s = sensor_data->ping[1];
        dist_l = sensor_data->ping[2];
        pause(100);
        //Continue until sees a gap or a while
        while(dist_s > 15  && dist_r > 15 && dist_l > 15)
        {
            //Start sensor
            ping->read();
            dist_r = sensor_data->ping[0];
            dist_s = sensor_data->ping[1];
            dist_l = sensor_data->ping[2];
            printf("L: %d, R: %d, S: %d\n",dist_l, dist_r, dist_s);
        }
       
        printf("Triggered\n");
        printf("L: %d, R: %d, S: %d\n",dist_l, dist_r, dist_s);

        //Update current position
        int orientation = driver->get_orientation();
        driver->update_position();
    
        //Set directional readings into coordinate positions
        int cardinal_arr[4];
        for (int i = 0; i < 4; i++) cardinal_arr[i]=0;

        int left_orient = 0;
        if ((orientation - 1) < 0) left_orient = (orientation+3) % 4;
        else left_orient = -1 * (orientation-1) % 4;
        cardinal_arr[left_orient] = dist_l;
        cardinal_arr[(orientation+1)%4] = dist_r;
        cardinal_arr[orientation % 4] = dist_s;
        
        //Send cardinal_arr to labview

        printf("Deciding\n");
        decide((int* )cardinal_arr); // Turn either left, right, or neither

    }
}
