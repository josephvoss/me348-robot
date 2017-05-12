#include <stdio.h>


//int main(int init_pos, int goal,int ff)
#include <stdio.h>


void ff_funct(int ff[6][6])
{
	int grid = 6;

	int walls[grid][grid];


	for(int i=0;i<grid;i++)
	{
	  for (int j=0;j<grid;j++)
	  {
	    ff[i][j] = 0;
	    //walls[i][j] = 0;
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
	  return;
}


int main(void)
{
	int init_pos[2];
	init_pos[0] = 1;
	init_pos[1] = 1;

	int goal[2];
	goal[0] = 2;
	goal[1] = 2;

	int ff_arr[6][6];

	ff_funct(ff_arr);

	int x = init_pos[0];
	int y = init_pos[1];
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

	printf("starting %d %d\n",init_pos[0],init_pos[1]);
	printf("goal %d %d\n",goal[0],goal[1]);

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

		printf("direction %d\n",dir);
		printf("pos %d %d\n",x,y);
	}


	printf("\n\nGOALREACHEDDDD\n\n");
}
	