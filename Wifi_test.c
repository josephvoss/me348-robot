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

        if (identifier == 100) waypointX = value;
        
        else if (identifier == 101) waypointY = value; 
        
        else if (identifier == 200) currentPosX = value;          
  
        else if (identifier == 201) currentPosY = value;
        
        printf("waypointX = %d\n", waypointX);
        printf("waypointY = %d\n", waypointY);
        printf("currentPosX = %d\n", currentPosX);
        printf("currentPosY = %d\n", currentPosY);
      }        
    }
    pause(500);
  }    
}
