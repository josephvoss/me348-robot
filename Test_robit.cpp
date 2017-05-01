#include <stdio.h>            // Recommended over iostream for saving space
#include <propeller.h>        // Propeller-specific functions

#include "Ping_sensor.h"
#include "Drive_wrapper.h"
#include "Control.h"

void init_all()
{
/* Benito Suggest
	Init, then wait to start until sonar reads 1cm, once hand moves start
	Also search for walls 
*/
    pause(2000);
    printf("Initialized Successfully\n");
}

void exit_all()
{
    printf("Exited Successfully\n");
}  

int main(){
    init_all();
    //IR and QTI not implemented, pins set to 0
    Ping_sensor turret(16, 19, 0,0,0,0);
    Drive_wrapper driver(0,0,0,0);
    Control controller(&turret, &driver);
    controller.main();
  
    return 0;
}

