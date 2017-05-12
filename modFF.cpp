#include <stdio.h>


int main(void)
{
int grid = 6;
int ff[grid][grid];

//int walls[grid][grid];
//int walls[6][6] = {{11,8,10,8,10,12},{9,6,9,2,12,5},{5,13,1,14,3,4},{0,1,5,9,12,5},{5,3,6,5,5,5},{3,10,10,6,3,6}};
int walls[6][6] = {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,2,0},{0,0,0,0,8,0},{0,0,0,4,5,1},{0,0,0,4,1,0}};

for(int i=0;i<grid;i++)
{
  for (int j=0;j<grid;j++)
  {
    ff[i][j] = 0;
    //walls[i][j] = 0;
  }
}

int n = 1;    //goal value
ff[5][0] = n; //setting location of goal

while (n < 50)
{
  for(int i=0; i<grid;i++)
  {
    for (int j=0; j<grid;j++)
    {
      if ((ff[i][j]) == n)    
      {
        
        if (i+1<grid)     //CHECK SOUTH
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

        if ((j+1)<grid)     //CHECK EAST
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

  for(int i=0;i<grid;i++)
  {
    for (int j=0;j<grid;j++)
    {
      //printf("%d\t",ff[i][j]);
      printf("%d\t",walls[i][j]);
    }
    printf("\n");
  }

  printf("\n\n");

  for(int i=0;i<grid;i++)
  {
    for (int j=0;j<grid;j++)
    {
      printf("%d\t",ff[i][j]);
      //printf("%d\t",walls[i][j]);
    }
    printf("\n");
  }



}
