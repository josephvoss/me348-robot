#include "simpletools.h"
#include "wifi.h"
#include "fdserial.h"

int event, id, handle;
int postFromPageId;

int waypointX, waypointY, currentPosX, currentPosY;

int main()
{
  wifi_start(9, 8, 115200, USB_PGM_TERM);

  postFromPageId = wifi_listen(HTTP, "/controller_post");
  printf("postFromPageId = %d\n", postFromPageId);

  while(1)
  {
    wifi_poll(&event, &id, &handle); 
    printf("event = %d, id = %d, handle = %d\r", event, id, handle);
    if(event == 'P')
    {
      if(id == postFromPageId)
      {
        printf("Incoming POST request\r");
        int identifier, value;
        wifi_scan(POST, handle, "set%d%d", &identifier, &value);
        printf("%d\t%d\n", identifier, value);

        int intPos = strlen(fullString)-1;
        char stringInt = fullString[intPos];
        if (strstr(fullString, "waypointX") != NULL)
        {
          waypointX = stringInt - 48;
          printf("WayX\n");
        }          
        
        else if (strstr(fullString, "waypointY") != NULL)
        {
          waypointY = stringInt - 48;
          printf("WayY\n");
        }          
        
        else if (strstr(fullString, "currentPosX") != NULL)
        {
          currentPosX = stringInt - 48;
          printf("posX\n");
        }                
  
        else if (strstr(fullString, "currentPosY") != NULL)
        {
          currentPosY = stringInt - 48;
          printf("posY\n");          
        }
        
        printf("waypointX = %d\n", waypointX);
        printf("waypointY = %d\n", waypointY);
        printf("currentPosX = %d\n", currentPosX);
        printf("currentPosY = %d\n", currentPosY);
      }        
    }
    pause(500);
  }    
}
