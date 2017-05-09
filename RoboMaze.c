/*
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
int *cog_stepUp;
int sonarPin,servoPin,irFrontPin,irLeftPin,irRightPin;
static volatile int stopStep,sonarAngle,sonarDis,sonarData[6],direction,stop,move,check;
static volatile float irFront,irLeft,irRight;

unsigned int stack[50];

void checkSignal(void *par)
{
  while(1)
  {
    freqout(11, 1, 38000);                      
    irLeft = input(1);                         
    freqout(1, 1, 38000);                       
    irRight = input(2);
    
    pause(1000);
    sonarDis = ping_cm(3);    
    
    if (sonarDis > 5 || irLeft == 1 || irRight == 1)
    {
      check = 0;
    }
    else if (sonarDis <= 5 || irLeft == 1 || irRight == 1)
    {
      check = 2;
    }
    else
    {check = 1;}      
  }
}                          

void sensorDec(void *par)
{
  irLeftPin=1;
  irRightPin=2;
  sonarPin=3;
  
  low(26);
  low(27);
  
  while(1)
  {
    freqout(11, 1, 38000);                      
    irLeft = input(irLeftPin);                   
    freqout(1, 1, 38000);                       
    irRight = input(irRightPin);
    
    pause(1000);
    sonarDis = ping_cm(sonarPin);    
    
    // Decision based on sensor data
    if (sonarDis > 5 && irLeft == 1 && irRight == 1) //forward
    {
      move = 1;
    }
    else if (sonarDis <= 5 && irLeft == 0 && irRight == 1) //turn left
    {
      move = 2;
    }
    else if (sonarDis <= 5 && irLeft == 1 && irRight == 0) //turn right
    {
      move=3;
    }
    else if (sonarDis > 5 && irLeft == 0 && irRight == 1) //forward or left
    {
      move=rand()%2+1;
    }
    else if (sonarDis > 5 && irLeft == 1 && irRight == 0) //forward or right
    {
      int a;
      a=rand()%2+1;
      if (a==2)
      {
        a=3;
      }
      move=a;
    }
    else if (sonarDis > 5 && irLeft == 0 && irRight == 0) //left or right
    {
      move=rand()%2+1;
    }         
    else if (sonarDis <= 5 && irLeft == 1 && irRight == 1) //turn around
    {
      move=4;
    }                                    
  }  
}    
        
void stepUp()
{
  drive_goto(100,100);
  pause(1000);
  stopStep=1;  
}

void driveForward()
{
  drive_speed(16,16);
  pause(1000);
  }    
  
void stopRobot()
{
  drive_speed(0,0);
  pause(500);
}  

void turn()
{
    switch (move){
      case 1 : //go straight
      drive_goto(100,100);
      pause(200);
      break;
      
      case 2: //turn left
      drive_goto(26,-25);
      pause(200);
      drive_goto(100,100);
      break;
      
      case 3: //turn right
      drive_goto(-26,25);
      pause(200);
      drive_goto(100,100);
      break;
      
      case 4: //turn around
      drive_goto(51,-51);
      pause(200);
      drive_goto(100,100);
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
  for (a1=90;a1=180;a1+5)
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
  for (a2=90;a2=0;a2-5)
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
            
//main loop
drive_trimSet(0,0,0);
while(1)
{
  driveForward();    
  int *cog_checkSignal = cog_run(checkSignal,128);
  if (check != 0)
  {    
    cog_end(cog_checkSignal);
    stopRobot();    
    if (check == 1)
    {
      stepUp();
    }  
    turn();
    selfOrient();   
  }
}      
}