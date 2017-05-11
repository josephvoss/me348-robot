#include "simpletools.h"
#include "wifi.h"
#include "fdserial.h"

int event, id, handle;
int postFromPageId;
int getFromPageId;

int waypointX, waypointY, currentPosX, currentPosY;

int main()
{
  wifi_start(9, 8, 115200, USB_PGM_TERM);

  //Start watcher for controller post
  postFromPageId = wifi_listen(HTTP, "/controller_post");
  printf("postFromPageId = %d\n", postFromPageId);

  //Start watcher for map get
  getFromPageId = wifi_listen(HTTP, "/map.html");
  printf("getFromPageId = %d\n", getFromPageId);
  
  //Init dummy wall
  int wall[6][6];
  for (int i=0; i<6; i++)
    for (int j=0; j<6; j++)
      wall[i][j] = i+j;

  while(1)
  {
    //Poll wifi module
    wifi_poll(&event, &id, &handle); 
    printf("event = %d, id = %d, handle = %d\r", event, id, handle);
    //If post and for controller
    if(event == 'P' && id == postFromPageId)
    {
        printf("Incoming POST request\r");
        int identifier, value;
        
        //Get identifier and the value for it
        wifi_scan(POST, handle, "set%d%d", &identifier, &value);
        printf("%d\t%d\n", identifier, value);

        //decode identifier and set to right variable    
        if (identifier == 100) waypointX = value;        
        else if (identifier == 101) waypointY = value;         
        else if (identifier == 200) currentPosX = value;            
        else if (identifier == 201) currentPosY = value;
        
        printf("waypointX = %d\n", waypointX);
        printf("waypointY = %d\n", waypointY);
        printf("currentPosX = %d\n", currentPosX);
        printf("currentPosY = %d\n", currentPosY);        
    }
    //If get and from map
    if(event == 'G' && id == getFromPageId)
    {
        printf("Incoming GET request\r");
        //Buffer
        char wall_string[150];
        int i, j;
        
        //Create string showing the walls
        for (int x=0; x<36; x++)
        {
          i = x / 6;
          j = x % 6;
          sprintf(wall_string+strlen(wall_string),"%d\t",wall[i][j]);
          if (j==5) sprintf(wall_string+strlen(wall_string),"\n");
        }          
        //Trying to rescue last integer
        //For w/e reason, last number is always dropped off
        sprintf(wall_string+strlen(wall_string),"1\t");

        //Print the walls
        wifi_print(GET, handle, "%s\n", wall_string);
        printf("%s", wall_string);
        for(int x=0; x<strlen(wall_string); x++) wall_string[x] = NULL;
        free(wall_string);
    }
    pause(500);
  }    
}
