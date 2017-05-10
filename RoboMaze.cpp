#include "simpletools.h"                      // Include simple tools
#include "servo.h"
#include "ping.h"
#include "adcDCpropab.h"
#include "abdrive.h"

static volatile int stopStep,sonarAngle,sonarDis,sonarData[6],direction,move,x,y;
unsigned int stack[50];

int ff_arr[6][6];
int walls[6][6];
int goal[2] = {2,2};

void modFF(int** ff, int* goal, int** walls)
/*
 * Modified flood fill. Used to solve directions of the matrix
 *
 * Inputs:
 *      2D array of flood fill values
 *      1D array of goal target location
 *      2D array of walls, in binary notation
 */
{


  //initialize ff matrix
  for(int i=0;i<6;i++)
  {
    for (int j=0;j<6;j++)
    {
      ff[i][j] = 0;
    }
  }

  int n = 1;    //ff maze goal
  ff[goal[0]][goal[1]] = n; //setting location of goal

  while (n < 12)    
  {
    for(int i=0; i<6;i++) 
    {
      for (int j=0; j<6;j++)
      {
        if ((ff[i][j]) == n)    
        {
          
          if (i+1<6)     //CHECK SOUTH
          {
            if (ff[i+1][j] != 1 && ff[i+1][j] == 0 && (walls[i][j] & 2) == 0)
            {
              ff[i+1][j] = n+1;
            }
          }

          if ((i-1)>-1)     //CHECK NORTH
          {
            if (ff[i-1][j] != 1 && ff[i-1][j] == 0 && (walls[i][j] & 8) == 0)
            {
              ff[i-1][j] = n+1;
            }
          }

          if ((j+1)<6)     //CHECK EAST
          {
            if (ff[i][j+1] != 1 && ff[i][j+1] == 0 && (walls[i][j] & 4)== 0)
            {
              ff[i][j+1] = n+1;
            }
          }

          if ((j-1)>-1)     //CHECK WEST
          {
            if (ff[i][j-1] != 1 && ff[i][j-1] == 0 && (walls[i][j] & 1) == 0)
            {
              ff[i][j-1] = n+1;
            }
          }
        }


      }
    }

    n++;
  }
    return;
}

void sensorDec()
/*
 * Sensor Decision. Makes a turn decision based on sensor readings.
 * Will be updated to use flood fill techniques
 *
 * Inputs:
 *      None?
 */
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
/*
 * Step Up. Causes robot to take a unit step (90 ticks, 292.5  mm, 11.5 inches) 
 * in the current direction.
 *
 */
{
  //Set ramping speed. Prevents overacceleration
  drive_setRampStep(2);

  //Drive 90 ticks in current direction
  drive_goto(90,90);
}

void turn()
/*
 * Turn. Turns robot depending on value of move. N = 0, E = 1, S = 2, W = 3.
 *
 * Inputs:
 *      None?!?
 */
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
/*
 * Re-orient the robot based on the distance between the two walls.
 *
 * Can we *please* change these variable names or add comments or something so
 * someone other than micheal understands what's going on?
 *
 */
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
/*
 * Updates the current direction of the robot.
 *
 * Outputs:
 *      None?!?
 */
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
/*
 * Updates the current position of the robot on a 6x6 cartesian grid
 *
 * Outputs:
 *      None?!?     
 */
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

int main()
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
