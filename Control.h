#include "Ping_sensor.h"
#include "Drive_wrapper.h"

typedef struct 
{
    int dir_arr[4];
    int pos_x;
    int pos_y;
} Decide_tuple;

class Control
{
    public:
        Control(Ping_sensor* ping_in, Drive_wrapper* drive_in);
        ~Control();
        void decide(int* arr);
        void main(void); // data needs to be of constant format
        Decide_tuple* decide_arr; // length 10 for now
    
    private:
        //Pointers to already-running submodules
        Ping_sensor* ping;
        Drive_wrapper* driver;
        //Pointer to pass to drive class
        int* drive_flag;
        //Pointer to pass to sense class
        int* sense_flag;

        //Decision_arr tracker
        int decide_count;
};
