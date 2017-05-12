/*9
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "servo.h"
#include "ping.h"
#include "adcDCpropab.h"
#include "abdrive.h"

int main()                                    // Main function
{
  void selfOrient()
{
  int leftPingAngle,leftPing,min_leftPing,leftRotateAngle,rightPingAngle,rightPing,min_rightPing,rightRotateAngle,rotateTicks;
  min_leftPing = 100;
  leftRotateAngle = 0;
  min_rightPing = 100;
  rightRotateAngle = 0;
  
  for (leftPingAngle=180 ; leftPingAngle>=90 ; leftPingAngle-=1) //scan the left side wall
  {
     servo_angle(16,leftPingAngle*10);
     pause(20);
     leftPing = ping_cm(17);
     if (leftPing < min_leftPing)
     {
       leftRotateAngle = 180-leftPingAngle;//angle to correct
       min_leftPing = leftPing;//store the minimum Ping sensor data
     }
     }
   
   servo_angle(16,0);
   pause(1000);   

           
  for (rightPingAngle=0 ; rightPingAngle<=90 ; rightPingAngle+=1) //scan the left side wall
  {
    servo_angle(16,rightPingAngle*10);
    pause(20);
    rightPing = ping_cm(17);
    if (rightPing < min_rightPing)
    {
      rightRotateAngle = rightPingAngle;//angle to correct
      min_rightPing = rightPing;//store the minimum sensor data
    }
   }
  
  if (leftRotateAngle-rightRotateAngle >= 10) //rotate the robot if difference larger than 10 degree
  {
    rotateTicks=leftRotateAngle*51/180;
    drive_goto(rotateTicks,-rotateTicks);
    pause(500); 
    } 
  else if (rightRotateAngle-leftRotateAngle >=10)
  {
    rotateTicks=rightRotateAngle*51/180;
    drive_goto(-rotateTicks,rotateTicks);
    pause(500);  
  }  
} 
  while(1)
  {
    // Add main loop code here.
    selfOrient();
    
  }  
}