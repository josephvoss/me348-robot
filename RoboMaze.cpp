#include "simpletools.h"                      // Include simple tools
#include "servo.h"
#include "ping.h"
#include "abdrive.h"
#include "wifi.h"

//#include "ff_functions.h"

//int position[2];
//int ff_arr[6][6];
//int wall_arr[6][6];
//int goal[2];
//int direction = 0;
//int move = 0;
//int wall_arr[6][6] = {{11,8,10,8,10,12},{9,6,9,2,12,5},{5,13,1,14,3,4},{1,4,5,9,12,5},{5,3,6,5,5,5},{3,10,10,6,3,6}};

void buildWall(int wall_arr[][6], int pos[], int direction)
/*
 * Build wall array for current position
 *
 * Inputs:
 *      2D array of walls in binary notation
 *      1D array of current location
 *      Integer of current direction
 */
{
  // N E S W - 8 4 2 1 -  0 1 2 3 
  int sonLeft,sonRight,sonarFront;
  int x = pos[0];
  int y = pos[1];

  //Read from ir sensors and ping sensor
  //freqout(11, 1, 38000);                      
  //irLeft = input(10);                   
  //freqout(1, 1, 38000);                       
  //irRight = input(2);
  servo_angle(16,0);
  pause(1000);
  sonRight = ping_cm(17);
 
  servo_angle(16,900);
  pause(1000);
  sonarFront = ping_cm(17);    
  
 
  servo_angle(16,1800);
  pause(1000);
  sonLeft = ping_cm(17);

  
  //Set wall value to a known value since the robot is here
  wall_arr[x][y] = 0;

  //Orient straight, left, and right in terms of the cardinal directions
  int s_dir = direction;
  int l_dir = 0;
  if ((direction - 1) < 0) l_dir = (direction - 1) % 4 + 4;
  else l_dir = (direction - 1) % 4;
  int r_dir = (direction + 1) % 4;

 // printf("%d\t%d\t%d\n",l_dir,s_dir,r_dir);
  printf("L R Str %d\t%d\t%d\n",sonLeft,sonarFront,sonRight);
  
  int sum = 0;

  //If walls exist add to array
  if (sonarFront < 20) //wall forward
  {
      
    wall_arr[x][y] += (int) pow(2,(int)3-s_dir);
  }
  if (sonLeft < 20) //wall left
  {
    wall_arr[x][y] += (int) pow(2,(int)3-l_dir);
  }
  if (sonRight < 20) //wall right
  {
    wall_arr[x][y] += (int) pow(2,(int)3-r_dir);
  }      

  int cell_update[4];

  for (int i=0;i<6;i++)
  {
    for (int j=0;j<6;j++)
    {
      for(int m=0;m<4;m++)
      {
        cell_update[m] = 0;
      }        
      
      if (wall_arr[i][j] == 0)
      {

        if (wall_arr[i-1][j] != 0 && i>0)  //north
        {
          if (wall_arr[i-1][j] == 2 || wall_arr[i-1][j] == 3 || wall_arr[i-1][j] == 6 || wall_arr[i-1][j] == 10 || wall_arr[i-1][j] == 7 || wall_arr[i-1][j] == 11 || wall_arr[i-1][j] == 14)
          {
            cell_update[0] = 1;
            //printf("cell update N %d\n",cell_update[0]);
          }
        }

        if (wall_arr[i][j+1] != 0 && j!=5)  // east
        {
          if (wall_arr[i][j+1] == 1 || wall_arr[i][j+1] == 3 || wall_arr[i][j+1] == 5 || wall_arr[i][j+1] == 7 || wall_arr[i][j+1] == 9 || wall_arr[i][j+1] == 11 || wall_arr[i][j+1] == 13)
          {
            cell_update[1] = 1;
            //printf("cell update E %d\n",cell_update[1]);
          }
          
        }

       if (wall_arr[i+1][j] != 0 && i!=5)  // south
       {
        if (wall_arr[i+1][j] == 8 || wall_arr[i+1][j] == 12 || wall_arr[i+1][j] == 14 || wall_arr[i+1][j] == 10 || wall_arr[i+1][j] == 11 || wall_arr[i+1][j] == 9 || wall_arr[i+1][j] == 13)
        {
          cell_update[2] = 1;
          //printf("cell update S %d\n",cell_update[2]);
        }
       }

       if (wall_arr[i][j-1] != 0 && j>0)  // west
       {
        if (wall_arr[i][j-1] == 4 || wall_arr[i][j-1] == 6 || wall_arr[i][j-1] == 7 || wall_arr[i][j-1] == 5 || wall_arr[i][j-1] == 12 || wall_arr[i][j-1] == 14 || wall_arr[i][j-1] == 13)
        {
          cell_update[3] = 1;
          //printf("cell update W %d\n",cell_update[3]);
        }
        
       }

       wall_arr[i][j] = cell_update[0]*8 + cell_update[1]*4 + cell_update[2]*2 + cell_update[3];
    
      }

    }
  }

}    



void stepUp()
/*
 * Step Up. Causes robot to take a unit step (90 ticks, 292.5  mm, 11.5 inches) 
 * in the current direction.
 *
 */
{

  /*
 *correct the deviation of the robot
 *
 *Add a sonar at the bottom left of the robot
 *
 */
  
  int leftWallDis_back,leftWallDis_front,adjust_counter;
  //Measure distance using the bottom left sonar
  leftWallDis_back=ping_cm(7); //assume the side sonarPin is 13
  if (leftWallDis_back < 20) //if there is a wall on the left
  {
    adjust_counter = 0;
    servo_angle(16,1800);
    leftWallDis_front=ping_cm(17);//Distance measure from the head sonar
    leftWallDis_back=ping_cm(7);//Distance from bottom sonar
    while (leftWallDis_front - leftWallDis_back > 5 && adjust_counter < 100) //assume 10, need to measure
    {
    drive_speed(-4,4);
    pause(5);
    leftWallDis_front=ping_cm(17);
    leftWallDis_back=ping_cm(7);
    printf("Adjusting Loop # %d\n",adjust_counter);
    adjust_counter++;
  }
    adjust_counter = 0;
    while (leftWallDis_front - leftWallDis_back < 5 && adjust_counter < 100)   
    {
      drive_speed(4,-4);
      pause(5);
      leftWallDis_front=ping_cm(17);
      leftWallDis_back=ping_cm(7);
      printf("Adjusting Loop # %d\n",adjust_counter);
      adjust_counter++;
    }      
    drive_speed(0,0);
    pause(10);
    servo_angle(16,900);
    pause(50);
  }    
  //Set ramping speed. Prevents overacceleration
  drive_setRampStep(1);

  //Drive 90 ticks in current direction
  servo_angle(16,900);
  //if(ping_cm(17) < 20)
  //{
   // printf("Wall too close, no step\n");
  //}
  //else{   
  drive_goto(90,90);
  //}  
}

void turn(int move)
/*
 * Turn. Turns robot depending on value of move.
 *
 * Inputs:
 *      Integer move. Range 0-3. For each case see structure below
 */

{
  drive_speed(0,0);
  switch (move){
    case 0 : //go straight  
    {
      printf("straight\n");
      break;
    } 

    case 1: //turn left
    printf("left\n");
    //drive_goto(-26,25);
    //pause(200)
    
    drive_ramp(-10,10);
    pause(2550);
    drive_ramp(0,0);
    break;
    
    case 2: //turn right
    printf("right\n");
    //drive_goto(26,-25);
    //pause(200);
    drive_ramp(10,-10);
    pause(2550);
    drive_ramp(0,0);
    break;
    
    case 3: //turn around
    printf("turn around\n");
    //drive_goto(51,-51);
    //pause(200);
    drive_ramp(-10,10);
    pause(2550);
    drive_ramp(0,0);
    drive_ramp(-10,10);
    pause(2550);
    drive_ramp(0,0);
    break;
  }      
}
  

int directionUpdate(int move, int currentDirection)
/*
 * Updates the current direction of the robot.
 *
 * Inputs:
 *      Integer move. Range 0-3. For each case see structure below.
 *      Integer current direction. Range 0-3, values are North-0 East-1 South-2 West-3
 *
 * Outputs:
 *      Integer with new value of direction
 */
{
  switch (move){
    case 0 : //go straight
    break;
    
    case 1: //turn left
    if (currentDirection == 0)
    {
      currentDirection = 3;
    }
    else {
      currentDirection-=1;     
    }   
    break;


    case 2: //turn right
    if (currentDirection == 3)
    {
      currentDirection = 0;
    }
    else {
      currentDirection+=1;     
    }           
    break;
    
    
    case 3: //turn around
    if (currentDirection==0 || currentDirection==1)
    {
      currentDirection+=2;
    }
    else {
      currentDirection-=2;
    }                
    break;
  }

  return currentDirection;    
}    
  
void positionUpdate(int move, int direction, int position[]) //(x,y)
/*
 * Updates the current position of the robot on a 6x6 cartesian grid
 *
 * Inputs
 *      Integer move/turn. Range 0-3. Contains value to turn to
 *      Integer direction. Range 0-3, values are North-0 East-1 South-2 West-3
 *      Integer pointer. Array with values of x and y. Contains current
 *          position data
 *
 */
{
  int x = position[0];
  int y = position[1];
  switch (move){
    case 0: //straight
    switch (direction){
      case 0:
      x-=1;break;
      case 1:
      y+=1;break;
      case 2:
      x+=1;break;
      case 3:
      y-=1;break;
    } break;
    case 1: //left    
    switch (direction){
      case 0:{
      y-=1;
      break;
    }
      case 1:
      x-=1;break;
      case 2:
      y+=1;break;
      case 3:
      x+=1;break;
    } break;
    case 2: //right
    switch (direction){
      case 0:
      y+=1;break;
      case 1:
      x+=1;break;
      case 2:
      y-=1;break;
      case 3:
      x-=1;break;
    } break;
    case 3: //backward
    switch (direction){
      case 0:
      x+=1;break;
      case 1:
      y-=1;break;
      case 2:
      x-=1;break;
      case 3:
      y+=1;break;
    } break; 
  }

  position[0] = x;
  position[1] = y;

  return;
}              

void wifiCheck(int event, int id, int handle, int postFromPageId, int getFromPageId, int goal[], int position[], int walls[][6], int ff[][6])
/*
 * Check wifi receiver for updates. Blocking, waits to receive a get
 * request before continuing.
 * 
 * Inputs
 *     Integer event, output from wifi_start
 *     Integer id, output from wifi_start
 *     Integer handle, output from wifi_start
 *     Integer id for post controller
 *     Integer id for wall data getter
 *     Integer array for position data
 *     Integer array for goal position
 *     Integer array for wall data
 * 
 */
{
    int getFlag = 0;
    while(getFlag != 1)
    {
      wifi_poll(&event, &id, &handle); 
//      printf("event = %c, id = %d, handle = %d\r", event, id, handle);
      //If post and for controller
      if(event == 'P' && id == postFromPageId)
      {
        int identifier, value;
        
        //Get identifier and the value for it
        wifi_scan(POST, handle, "set%d%d", &identifier, &value);

        printf("%d\t%d\n", identifier, value);

        //decode identifier and set to right variable    
        if (identifier == 100) goal[0] = value;        
        else if (identifier == 101) goal[1] = value;         
        else if (identifier == 200) position[0] = value;            
        else if (identifier == 201) position[1] = value;      
      }
      //If get and from map
      if(event == 'G' && id == getFromPageId)
      {
        //Buffer
        char wall_string[400];
        int i, j;
        
        //Create string showing the walls
        for (int x=0; x<36; x++)
        {
          i = x / 6;
          j = x % 6;
          sprintf(wall_string+strlen(wall_string),"%d\t",walls[i][j]);
          if (j==5) sprintf(wall_string+strlen(wall_string),"\n");
          
        }          
        sprintf(wall_string+strlen(wall_string),"\n");
        //Create string showing the ff
        for (int x=0; x<36; x++)
        {
          i = x / 6;
          j = x % 6;
          sprintf(wall_string+strlen(wall_string),"%d\t",ff[i][j]);
          if (j==5) sprintf(wall_string+strlen(wall_string),"\n");
        }          
        //Trying to rescue last integer
        //For w/e reason, last number is always dropped off
        sprintf(wall_string+strlen(wall_string),"1\t");
        sprintf(wall_string+strlen(wall_string),"1\t");

        //Print the walls
        wifi_print(GET, handle, "%s\n", wall_string);
        printf("Wifi is seeing ---\n");
        printf("%s\n", wall_string);
        for(int x=0; x<strlen(wall_string); x++) wall_string[x] = NULL;
        
        //Exit loop
        getFlag = 1;
        printf("Set getFlag\n");
      }
      //printf("getFlag=%d\n", getFlag);      
      pause(500); 
    }            
}  
  
void adjustPosition()
/*
 *adjust the positon to avoid colision
 */
{
  int wallFrontDis;
  servo_angle(16,900);
  //Measure distance using the bottom left sonar
  wallFrontDis=ping_cm(17); 
  if (wallFrontDis < 10) 
  {
    int adjust_counter = 0;
    while (wallFrontDis > 5 && adjust_counter < 100) 
    {
    drive_speed(4,4);
    pause(5);
    wallFrontDis=ping_cm(17);
    printf("Adjusting Loop # %d\n",adjust_counter);
    adjust_counter++;
  }
    adjust_counter = 0;
    while (wallFrontDis < 5 && adjust_counter < 100)   
    {
      drive_speed(-4,-4);
      pause(5);
      wallFrontDis=ping_cm(17);
      printf("Adjusting Loop # %d\n",adjust_counter);
      adjust_counter++;
    }      
    drive_speed(0,0);
    pause(10);
  }    
}  
 

int main()
{
  //Initialize variables to 0.
  int ff_arr[6][6];
  int wall_arr[6][6];
  int goal[2];       // [x,y]
  int direction = 0; // [N=0, E=1, S=2, W=3]
  int move = 0;      // [For=0, Left=1, Right=2, 180=3]
  int position[2];   // [x (dwn),y (across)]
  
  
  //initialize ff & wall arrays to 6x6 zeros
  for (int i=0; i<6; i++)
    for (int j=0; j<6; j++)
    {
      ff_arr[i][j] = 0;
      wall_arr[i][j] = 0;
    }      
    
  //set encoder and servo pins
  drive_servoPins(12,13);
  drive_encoderPins(14, 15);
  
  //Init wifi hardware
  wifi_start(9, 8, 115200, USB_PGM_TERM);
  pause(500);
  int event, id, handle;
  
  //Start watcher for controller post
  int postFromPageId = wifi_listen(HTTP, "/controller_post");
  printf("postFromPageId = %d\n", postFromPageId);

  //Start watcher for map get
  int getFromPageId = wifi_listen(HTTP, "/map.html");
  printf("getFromPageId = %d\n", getFromPageId);  


  position[0] = -1;      //Set intial x to 0
  position[1] = -1;      //Set intial y to 0

  //Set goal
  goal[0] = 3;
  goal[1] = 2;

  //Wait until position is set by controller
  while( position[1] == -1 )
  {
    wifiCheck(event, id, handle, postFromPageId, getFromPageId, goal, position, wall_arr, ff_arr);
  }

  // MAIN LOOP
  while(1)
  {    
    
    //Sense around robot
    buildWall(wall_arr, position, direction);

    printf("Wall Array\n");
    for (int i=0; i<6; i++)
    {
      for(int j=0; j<6; j++)
      {
        printf("%d\t", wall_arr[i][j]);
        ff_arr[i][j]=0; //need to initialize back to 0
      }
      printf("\n");
    }     
    
    //Decide where to go 
    //ff_funct(ff_arr, goal, wall_arr);

    ////FF BUILDER
    int n = 1;    //goal value
    ff_arr[goal[0]][goal[1]] = n; //setting location of goal

    while (n < 50)
    {
      for(int i=0; i<6;i++)
      {
        for (int j=0; j<6;j++)
        {
          if ((ff_arr[i][j]) == n)    
          {
            
            if (i+1<6)     //CHECK SOUTH
            {
              if (ff_arr[i+1][j] != 1 && ff_arr[i+1][j] == 0 && (wall_arr[i][j] & 2) == 0)
              {
                ff_arr[i+1][j] = n+1;
              }
            }

            if ((i-1)>-1)     //CHECK NORTH
            {
              if (ff_arr[i-1][j] != 1 && ff_arr[i-1][j] == 0 && (wall_arr[i][j] & 8) == 0)
              {
                ff_arr[i-1][j] = n+1;
              }
            }

            if ((j+1)<6)     //CHECK EAST
            {
              if (ff_arr[i][j+1] != 1 && ff_arr[i][j+1] == 0 && (wall_arr[i][j] & 4)== 0)
              {
                ff_arr[i][j+1] = n+1;
              }
            }

            if ((j-1)>-1)     //CHECK WEST
            {
              if (ff_arr[i][j-1] != 1 && ff_arr[i][j-1] == 0 && (wall_arr[i][j] & 1) == 0)
              {
                ff_arr[i][j-1] = n+1;
              }
            }
          }


        }
      }

      n++;
    }
    
    printf("FF Array\n");
    for (int i=0; i<6; i++)
    {
      for(int j=0; j<6; j++)
      {
        printf("%d\t", ff_arr[i][j]);
      }
      printf("\n");
    }     
    
    

      //FF FOLLOW
      int x = position[0];
      int y = position[1];
      int ff_value[4];
      int lowest = 256;	
      int card = 256; //out of the way value
      int move;
      
      //populates ff_value 
      //N E S W -- 0 1 2 3 

      for(int i =0;i<4;i++){
        ff_value[i] = 256;
      }

      if (x-1 > -1)     //CHECK NORTH
      {
        ff_value[0] = ff_arr[x-1][y];
      }

      if (y+1<6)    //CHECK EAST
      {
        ff_value[1] = ff_arr[x][y+1];
      }

      if (x+1 < 6) 		//CHECK SOUTH
      {
      	ff_value[2] = ff_arr[x+1][y];
      }
      
      
      if (y-1 > -1) 		//CHECK WEST
      {
      	ff_value[3] = ff_arr[x][y-1];
      }
      
      //Print FF Value
      // for (int i = 0; i<4; i++)
      //   printf("FF %d\t %d\n", i, ff_value[i]);
      
      // for (int i =0;i<4;i++){
      //     if (ff_arr[y][x] - 1 == ff_value[i])
      //     {
      //       card = i;
      //     }
      //   }
      //printf("x y %d %d\n", x,y);

      //finds lowest value which is the next direction
        //if there are ties, default N,E,S,W tiebreakers
      for (int i=0;i<4;i++)
      {
      	if ((ff_value[i] < lowest && i==0 ) && !(wall_arr[x][y] & 8))
      	{
      	  lowest = ff_value[i];
      	  card = i;	//this tells you where to move
      	}

        if ((ff_value[i] < lowest && i==1 ) && !(wall_arr[x][y] & 4))
        {
          lowest = ff_value[i];
          card = i; //this tells you where to move
        }

        if ((ff_value[i] < lowest && i==2) && !(wall_arr[x][y] & 2))
        {
          lowest = ff_value[i];
          card = i; //this tells you where to move
        }

        if ((ff_value[i] < lowest && i==3 ) && !(wall_arr[x][y] & 1))
        {
          lowest = ff_value[i];
          card = i; //this tells you where to move
        }
      }
      //printf("lowest %d\n",lowest);
      lowest = 256;
      
      //tells robot which ff_value to move
       //printf("CARD IS %d\n",card);
        


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
        
    //move is a turn where 0 1 2 3 == S L R 180
    //move = ff_follower(position, goal, ff_arr,direction, wall_arr); 
    printf("\n");
    
    
    //Poll wifi module
    wifiCheck(event, id, handle, postFromPageId, getFromPageId, goal, position, wall_arr, ff_arr);
    

    
    //EXECUTE TURN & MOVEMENT 
    //Turn if needed
    turn(move); 
//    printf("\nMOVEMOVEMOVEMOVMEOVOE %d\n",move);   
    
    //Move forward 1 unit
    stepUp();
    
    //printf("stepped\n");

    //Adjust position
    adjustPosition();
    
    //Update direction and position
    printf("Move is %d\n", move);
    //direction = directionUpdate(move, direction);
    positionUpdate(move,direction, position);
    direction = directionUpdate(move, direction);
    printf("Direction is %d\n", direction);
    printf("Position is %d %d\n", position[0],position[1]);

    
    printf("Goal is %d %d \n",goal[0],goal[1]);
    
    if (ff_arr[position[0]][position[1]] == 1)
    {
      drive_speed(-55,55);
      pause(5000);
      drive_speed(0,0);
      printf("YOU DID IT");
      break;
    }    
    pause(1000);
  }   
}
