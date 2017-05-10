/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "servo.h"
#include "ping.h"
#include "adcDCpropab.h"
#include "abdrive.h"


int ff_arr[6][6];
int wall_arr[6][6];
int goal[2] = {2,2};
int pos[2] = {5,5};
int orientation = 0;

void ff_funct(int** ff, int* goal, int** walls)
{
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

int ff_follower(int* pos, int* goal, int** ff_arr)
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


	//finds lowest value which is the next direction
  //if there are ties, default N,E,S,W tiebreakers
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
  pos[0] = x;
  pos[1] = y;
  return dir;
}
	
void buildWall(int** wall_arr, int* pos, int* orientation)
{
  // N E S W - 8 4 2 1 -  0 1 2 3 
  int irLeft,irRight,sonarDis;
  int x = pos[0];
  int y = pos[1];

  low(26);
  low(27);

  freqout(11, 1, 38000);                      
  irLeft = input(10);                   
  freqout(1, 1, 38000);                       
  irRight = input(2);
  sonarDis = ping_cm(17);    
  
  wall_arr[x][y] = 0;

  int direction = *orientation;

  int s_dir = direction;
  int l_dir;
  if ((direction - 1) < 0) l_dir = (direction - 1 + 4) % 4;
  else l_dir = (direction - 1) % 4;
  int r_dir = (direction + 1) % 4;
  
  // Decision based on sensor data
  if (sonarDis > 5 && irLeft == 0 && irRight == 0) //forward
  {
    wall_arr[x][y] += pow(2,3-s_dir);
  }
  else if (sonarDis <= 5 && irLeft == 1 && irRight == 0) //turn left
  {
    wall_arr[x][y] += pow(2,3-l_dir);
  }
  else if (sonarDis <= 5 && irLeft == 0 && irRight == 1) //turn right
  {
    wall_arr[x][y] += pow(2,3-r_dir);
  }                                    
}    
        

//main loop
int main()                                    // Main function
{  
  //initialize ff/wall_arr 
  for(int i=0;i<6;i++)
  {
    for (int j=0;j<6;j++)
    {
      ff_arr[i][j] = 256;
      wall_arr[i][j]=256;
      
    }
  }

  while(1)
  {
    ff_funct((int**) ff_arr,goal, (int**) wall_arr);
    ff_follower(pos, goal, (int**) ff_arr);
  }    
}

