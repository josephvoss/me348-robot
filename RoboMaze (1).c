/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "servo.h"
#include "ping.h"
#include "adcDCpropab.h"
#include "abdrive.h"

static volatile int stopStep,sonarAngle,sonarDis,sonarData[6],direction,move,check,x,y;
unsigned int stack[50];

void checkSignal()
{
  int irLeft,irRight,sonarDis;

    freqout(11, 1, 38000);                      
    irLeft = input(10);                         
    freqout(1, 1, 38000);                       
    irRight = input(2);
    sonarDis = ping_cm(17);    
    
    if (sonarDis > 20 && irLeft == 0 && irRight == 0)
    {
      check = 0;
      pause(300);
    }
    else if (sonarDis <= 20 && irLeft == 0 && irRight == 0)
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
    if (sonarDis > 20 && irLeft == 0 && irRight == 0) //forward
    {
      move = 1;
    }
    else if (sonarDis <= 20 && irLeft == 1 && irRight == 0) //turn left
    {
      move = 2;
    }
    else if (sonarDis <= 20 && irLeft == 0 && irRight == 1) //turn right
    {
      move=3;
    }
    else if (sonarDis > 20 && irLeft == 1 && irRight == 0) //forward or left
    {
      move=rand()%2+1;
    }
    else if (sonarDis > 20 && irLeft == 0 && irRight == 1) //forward or right
    {
      int a;
      a=rand()%2+1;
      if (a==2)
      {
        a=3;
      }
      move=a;
    }
    else if (sonarDis > 20 && irLeft == 1 && irRight == 1) //left or right
    {
      move=rand()%2+1;
    }         
    else if (sonarDis <= 20 && irLeft == 0 && irRight == 0) //turn around
    {
      move=4;
    }                                     
}    
        
void stepUp()
{
  drive_setRampStep(2);
  drive_goto(90,90);
  
}

void turn()
{
    switch (move){
      case 1 : //go straight
      drive_goto(10,10);
      pause(200);
      break;
      
      case 2: //turn left
      drive_goto(-26,25);
      pause(200);
      break;
      
      case 3: //turn right
      drive_goto(26,-25);
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
  for (a1=135;a1<=180;a1+=5)
  {
     servo_angle(16,a1*10);
     pause(100);
     b1 = ping_cm(17);
     if (b1 <= c1)
     {
       d1 = 180-a1;
       c1 = b1;
     }
   }
   servo_angle(16,900);
   pause(1000);   

           
  for (a2=45;a2>=0;a2-=5)
  {
    servo_angle(16,a2*10);
    pause(100);
    b2 = ping_cm(17);
    if (b2 <= c2)
    {
      d2 = a2;
      c2 = b2;
    }
  }
    servo_angle(16,900);
    if (d1 >= d2)
  {
    rotateTicks=d1*51/180;
    drive_goto(rotateTicks,-rotateTicks);
    pause(500); 
  
} 
  else if (d1 < d2)
  {
 rotateTicks=d2*51/180-4;
  drive_goto(-rotateTicks,rotateTicks);
  pause(500);  
  }  
} 
       
void directionUpdate() //North-0 East-1 South-2 West-3
{
    switch (move){
      case 1 : //go straight
      break;
      
      case 2: //turn left
      if (direction == 0)
      {
        direction = 3;
      }
      else {
        direction-=1;     
      }           
      break;
      
      case 3: //turn right
      if (direction == 3)
      {
        direction = 0;
      }
      else {
        direction+=1;     
      }   
      break;
      
      case 4: //turn around
      if (direction==0 || direction==1)
      {
        direction+=2;
      }
      else {
        direction-=2;
      }                
      break;
    }      
  }    
  
void positionUpdate() //(x,y)
{
  switch (move){
    case 1:
    switch (direction){
      case 0:
      y+=1;break;
      case 1:
      x+=1;break;
      case 2:
      y-=1;break;
      case 3:
      x-=1;break;
    }
    case 2:      
    switch (direction){
      case 0:
      x-=1;break;
      case 1:
      y+=1;break;
      case 2:
      x+=1;break;
      case 3:
      y-=1;break;
    }  
    case 3:
    switch (direction){
      case 0:
      x+=1;break;
      case 1:
      y-=1;break;
      case 2:
      x-=1;break;
      case 3:
      y+=1;break;
    }  
    case 4:
    switch (direction){
      case 0:
      y-=1;break;
      case 1:
      x-=1;break;
      case 2:
      y+=1;break;
      case 3:
      x+=1;break;
    }  
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
direction=0;
x=0;
y=0;
//drive_trimSet(0,0,0);
while(1)
{
  selfOrient();
  stepUp();  
  sensorDec();
  turn();    
  positionUpdate();
  directionUpdate();
}    
}