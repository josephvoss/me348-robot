#include "ff_functions.h"
#include "simpletools.h"

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
//  int walls[6][6] = {{11,8,10,8,10,12},{9,6,9,2,12,5},{5,13,1,14,3,4},{1,4,5,9,12,5},{5,3,6,5,5,5},{3,10,10,6,3,6}};
  printf("wall array");
  for (int i=0; i<6; i++)
  {
    for (int j=0; j<6; j++)
      printf("%d\t", walls[i][j]);
    printf("\n");
  }

  int n = 1;    //ff maze goal
  ff[goal[0]][goal[1]] = n; //setting location of goal

  while (n < 50)    
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

int ff_follower(int pos[], int goal[], int ff_arr[][6], int direction, int walls[][6])
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
  int lowest = 256;	
  int card = 256; //out of the way value
  int move;
  

  //for (int i=0;i<4;i++)
  //{
  	//ff_value[i] = 256;	//ff_value initialized to high number
 // }
  
  //populates ff_value 
  if (x+1 < 6) 		//CHECK SOUTH
  {
  	ff_value[2] = ff_arr[x+1][y];
  }
  
  if (x-1 > -1) 		//CHECK NORTH
  {
  	ff_value[0] = ff_arr[x-1][y-1];
  }
  
  if (y+1<6) 		//CHECK EAST
  {
  	ff_value[1] = ff_arr[x][y+1];
  }
  
  if (y-1 > -1) 		//CHECK WEST
  {
  	ff_value[3] = ff_arr[x][y-1];
  }
  
  for (int i = 0; i<4; i++)
    printf("FF %d\t %d\n", i, ff_value[i]);
  
  // for (int i =0;i<4;i++){
  //     if (ff_arr[y][x] - 1 == ff_value[i])
  //     {
  //       card = i;
  //     }
  //   }
  printf("x y %d %d\n", x,y);

  //finds lowest value which is the next direction
    //if there are ties, default N,E,S,W tiebreakers
  for (int i=0;i<4;i++)
  {
  	if ((ff_value[i] < lowest && i==0 ) && !(walls[x][y] & 8))
  	{
  	  lowest = ff_value[i];
  	  card = i;	//this tells you where to move
  	}

    if ((ff_value[i] < lowest && i==1 ) && !(walls[x][y] & 4))
    {
      lowest = ff_value[i];
      card = i; //this tells you where to move
    }

    if ((ff_value[i] < lowest && i==2) && !(walls[x][y] & 2))
    {
      lowest = ff_value[i];
      card = i; //this tells you where to move
    }

    if ((ff_value[i] < lowest && i==3 ) && !(walls[x][y] & 1))
    {
      lowest = ff_value[i];
      card = i; //this tells you where to move
    }
  }
  printf("lowest %d\n",lowest);
  lowest = 256;
  
  //tells robot which ff_value to move
    printf("CARD IS %d\n",card);
    

  // switch(card)
  // {
  // 	case 0: //north
  // 	{
  //     y--;
  //     break;
  // 	}
  
  // 	case 1: //east
  // 	{
  //     x++;
  // 	  break;
  // 	}
  
  // 	case 2: //south
  // 	{
  //     y++;
  // 	  break;
  // 	}	
  // 	case 3: //west
  // 	{
  // 	  x--;
  //     break;
  // 	}	
  // }

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

  printf("MOVE is %d\n", move);
  printf("DIR is %d\n", direction);

  return move;
}
