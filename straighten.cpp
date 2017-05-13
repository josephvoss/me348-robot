/*
  Straighten Function
*/
#include "simpletools.h"                      // Include simple tools
#include "ping.h"
#include "abdrive.h"
#include "servo.h"

void straighten()
/*
 *correct the deviation of the robot
 *
 *Add a sonar at the bottom left of the robot
 *
 */
{
  int leftWallDis_back,leftWallDis_front;
  //Measure distance using the bottom left sonar
  leftWallDis_back=ping_cm(13); //assume the side sonarPin is 13
  if (leftWallDis_back < 20) //if there is a wall on the left
  {
    servo_angle(16,1800);
    leftWallDis_front=ping(17);//Distance measure from the head sonar
    leftWallDis_back=ping(13);//Distance from bottom sonar
    while (leftWallDis_front - leftWallDis_back > 10) //assume 10, need to measure
    {
    drive_speed(-4,4);
    pause(5);
    leftWallDis_front=ping(17);
    leftWallDis_back=ping(13);
  }
    while (leftWallDis_front - leftWallDis_back < 10)   
    {
      drive_speed(4,-4);
      pause(5);
      leftWallDis_front=ping(17);
      leftWallDis_back=ping(13);
    }      
    drive_speed(0,0);
    pause(10);
  }    
}  





int main()
{}