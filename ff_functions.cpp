#include "ff_functions.h"

void ff_funct(int ff[][6], int goal[], int walls[][6])
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

int ff_follower(int pos[], int goal[], int ff_arr[][6], int direction)
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
  int card = 256; //out of the way value
  int card_arr[50];
  int move;
  
  for (int i=0;i<50;i++)
  {
    card_arr[i] = 256; 	//set to high value
  }
  
  for (int i=0;i<4;i++)
  {
  	ff_value[i] = 256;	//ff_value initialized to high number
  }
  
  //populates ff_value 
  if (y+1 < 6) 		//CHECK SOUTH
  {
  	ff_value[2] = ff_arr[y+1][x];
  }
  
  if (y-1 > -1) 		//CHECK NORTH
  {
  	ff_value[0] = ff_arr[y-1][x];
  }
  
  if (x+1<6) 		//CHECK EAST
  {
  	ff_value[1] = ff_arr[y][x+1];
  }
  
  if (x-1 > -1) 		//CHECK WEST
  {
  	ff_value[3] = ff_arr[y][x-1];
  }
  
  
  //finds lowest value which is the next direction
    //if there are ties, default N,E,S,W tiebreakers
  for (int i=0;i<4;i++)
  {
  	if (ff_value[i] < lowest)
  	{
  	  lowest = ff_value[i];
  	  card = i;	//this tells you where to move
  	}
  }
  
  lowest = 256;
  
  //tells robot which ff_value to move
  switch(card)
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

  //change card to a dir

  //go straight
  if (card == direction)
  {
    move = 0;
  }

  //turn left
  if (card == 3 && direction==0){
    move = 1;
  }
  else if ((card+1) == direction)
  {
    move = 1;
  }

  //turn right
  if (card == 0 && direction == 3){
    move = 2;    
  }

  else if ((card-1)==direction)
  {
    move = 2;
  }

  //turn 180
  if ((card - direction == -2) || (card-direction == 2))
  {
    move = 3;
  }

  
  pos[0] = x;
  pos[1] = y;
  return move;
}
