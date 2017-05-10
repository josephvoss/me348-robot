#include <stdio.h>


int main(void)
{
int grid = 6;
int ff[grid][grid];

int walls[grid][grid];


for(int i=0;i<grid;i++)
{
  for (int j=0;j<grid;j++)
  {
    ff[i][j] = 0;
    walls[i][j] = 0;
  }
}

int n = 1;    //goal value
ff[2][2] = n; //setting location of goal

while (n < 12)
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
      printf("%d\t",ff[i][j]);
    }
    printf("\n");
  }

}
