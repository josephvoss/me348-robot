/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "servo.h"
#include "ping.h"
#include "adcDCpropab.h"
#include "abdrive.h"

static volatile int stopStep,sonarAngle,sonarDis,sonarData[6],direction,move,check;
unsigned int stack[50];

void checkSignal()
{
  int irLeft,irRight,sonarDis;

    freqout(11, 1, 38000);                      
    irLeft = input(10);                         
    freqout(1, 1, 38000);                       
    irRight = input(2);
    sonarDis = ping_cm(17);    
    
    if (sonarDis > 5 && irLeft == 0 && irRight == 0)
    {
      check = 0;
      pause(300);
    }
    else if (sonarDis <= 5 && irLeft == 0 && irRight == 0)
    {
      check = 2;
    }
    else
    {check = 1;
    }      
}                          

void sensorDec()
{
  int irLeft,irRight,sonarDis;
  low(26);
  low(27);
  
    freqout(11, 1, 38000);                      
    irLeft = input(10);                   
    freqout(1, 1, 38000);                       
    irRight = input(2);
    sonarDis = ping_cm(17);    
    
    // Decision based on sensor data
    if (sonarDis > 5 && irLeft == 0 && irRight == 0) //forward
    {
      move = 1;
    }
    else if (sonarDis <= 5 && irLeft == 1 && irRight == 0) //turn left
    {
      move = 2;
    }
    else if (sonarDis <= 5 && irLeft == 0 && irRight == 1) //turn right
    {
      move=3;
    }
    else if (sonarDis > 5 && irLeft == 1 && irRight == 0) //forward or left
    {
      move=rand()%2+1;
    }
    else if (sonarDis > 5 && irLeft == 0 && irRight == 1) //forward or right
    {
      int a;
      a=rand()%2+1;
      if (a==2)
      {
        a=3;
      }
      move=a;
    }
    else if (sonarDis > 5 && irLeft == 1 && irRight == 1) //left or right
    {
      move=rand()%2+1;
    }         
    else if (sonarDis <= 5 && irLeft == 0 && irRight == 0) //turn around
    {
      move=4;
    }                                     
}    
        
void stepUp()
{
  drive_goto(1,1);
}

void turn()
{
    switch (move){
      case 1 : //go straight
      drive_goto(10,10);
      pause(200);
      break;
      
      case 2: //turn left
      drive_goto(26,-25);
      pause(200);
      break;
      
      case 3: //turn right
      drive_goto(-26,25);
      pause(200);
      break;
      
      case 4: //turn around
      drive_goto(51,-51);
      pause(200);
      break;
    }      
}
  
void selfOrient()
{
  int a1,b1,c1,d1,a2,b2,c2,d2,rotateTicks;
  c1 = 100;
  d1 = 0;
  c2 = 100;
  d2 = 0;
  for (a1=90;a1<=180;a1=+5)
  {
     servo_angle(16,a1);
     pause(100);
     b1 = ping_cm(3);
     if (b1 < c1)
     {
       d1 = 90-a1;
       c1 = b1;
     }
   }
   servo_angle(16,90);
   pause(500);          
  for (a2=90;a2<=0;a2=-5)
  {
    servo_angle(16,a2);
    pause(100);
    b2 = ping_cm(3);
    if (b2 < c2)
    {
      d2 = 90 - a2;
      c2 = b1;
    }
  }
  servo_angle(16,90);
  pause(500);
  if (d1 <= d2)
  {
    rotateTicks=d1*51/180;
    drive_goto(-rotateTicks,rotateTicks);
    pause(500);
  }
  else if (d1 > d2)
  {
    rotateTicks=d2*51/180;
    drive_goto(rotateTicks,-rotateTicks);
    pause(500);
  }
}          
            
/*void toCenter()
{
  int left,right,difference;
  servo_angle(16,180);
  pause(500);
  left = ping_cm(3);
  servo_angle(16,0);
  pause(1000);
  right = ping_cm(3);
}  */

//main loop
int main()                                    // Main function
{  

//drive_trimSet(0,0,0);
while(1)
{
  stepUp();  
  sensorDec();
  turn();  
}    
}