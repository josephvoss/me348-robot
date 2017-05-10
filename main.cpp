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

int ff_arr[6][6];
int wall_arr[6][6];
int goal[2] = {2,2};
int pos[2] = {5,5};

void ff_funct(int** ff, int* goal, int** walls)
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

void ff_follower(int* pos, int* goal, int** ff_arr)
{

	int x = pos[0];
	int y = pos[1];
	int ff_value[4];
	int lowest = 12;	//corresponds to "n"
	int dir= 256; //out of the way value
	int dir_arr[50];

	for (int i=0;i<50;i++)
	{
		dir_arr[i] = 256; 	//set to high value
	}

	for (int i=0;i<4;i++)
	{
		ff_value[i] = 256;	//ff_value initialized to high number
	}


	while (x != goal[0] || y != goal[1]) 	
	{
		
		//populates ff_value 
		if (y+1 < 6) 		//CHECK SOUTH
		{
			ff_value[2] = ff_arr[x][y+1];
			
		}
		
		if (y-1 > -1) 		//CHECK NORTH
		{
			ff_value[0] = ff_arr[x][y-1];
			
		}

		if (x+1<6) 		//CHECK EAST
		{
			ff_value[1] = ff_arr[x+1][y];
			
		}

		if (x-1 > -1) 		//CHECK WEST
		{
			ff_value[3] = ff_arr[x-1][y];
			
		}


		//finds lowest value
		for (int i=0;i<4;i++)
		{
			if (ff_value[i] < lowest)
			{
				lowest = ff_value[i];
				dir= i;	//this tells you where to move
			}
		}

		lowest = 256;


		//tells robot which ff_value to move
		switch(dir)
		{
			case 0: //north
			{y--;
	
			break;
			}

			case 1: //east
			{x++;
			
			break;
			}

			case 2: //south
			{y++;
			
			break;
			}	
			case 3: //west
			{
				x--;
		
				break;
			}	
		}
	}
  return;
}
	

void checkSignal()
{
  int irLeft,irRight,sonarDis;
    //wall is 0, nothing is 1
    freqout(11, 1, 38000);                      
    irLeft = input(10);                         
    freqout(1, 1, 38000);                       
    irRight = input(2);
    sonarDis = ping_cm(17);    
    
    if (sonarDis > 5 && irLeft == 0 && irRight == 0)
    {
      //can only go straight
      check = 0;
      pause(300);
    }
    else if (sonarDis <= 5 && irLeft == 0 && irRight == 0)
    {
      //dead end, turn back
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
  stepUp();  
  sensorDec();
  turn();    
  positionUpdate();
  directionUpdate();
  ff_funct((int**) ff_arr,goal, (int**) wall_arr);
  ff_follower(pos, goal, (int**) ff_arr);

}    
}