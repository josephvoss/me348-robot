#include "ff_functions.h"

void ff_funct(int** ff, int* goal, int** walls)
/*
 * Modified flood fill. Used to solve directions of the matrix
 *
 * Inputs:
 *      2D array of flood fill values
 *      1D array of goal target location
 *      2D array of walls, in binary notation
 */
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
/*
 * Flood Fill Follower? - Takes a filled ff array and returns a position it needs
 * to go
 * 
 * Inputs:
 *      1D array of current location
 *      1D array of goal target location
 *      2D array of flood fill values
 *
 * Outputs:
 *      Integer direction to turn
 */
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
  	{
      y--;
      break;
  	}
  
  	case 1: //east
  	{
      x++;
  	  break;
  	}
  
  	case 2: //south
  	{
      y++;
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
