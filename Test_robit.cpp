#include <stdio.h>            // Recommended over iostream for saving space
#include <propeller.h>        // Propeller-specific functions

#include "Control.h"

//IR and QTI not implemented, pins set to 0
Ping_sensor turret(16, 19, 0, 0, 0, 0);
Drive_wrapper driver(12, 13, 14, 15);

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

void sense_run(void*)
{
   turret.run();
}  

void drive_run(void*)
{
    driver.run();
}  

int main()
{
    init_all();

    Control controller(&turret, &driver);
    cog_run(*sense_run, 100);
    cog_run(*drive_run, 100);
    controller.main();
  
    return 0;
}

