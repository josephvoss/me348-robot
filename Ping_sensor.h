#include "ping.h"
#include "servo.h"

class Ping_sensor
{
  public:
    Ping_sensor(int pin_s, int pin_p, int pin_lir, int pin_rir, int pin_lqt, int pin_rqt, int number);
    ~Ping_sensor();
    void read();
    void run();//Function to be run on cog with class in shared memory (right?)
    int* distance_arr; //made public for shared memory testing. Should be
                       //private w/ mutator functions
  
  private:
    int pin_servo;
    int pin_ping;
    int pin_left_ir;
    int pin_right_ir;
    int pin_left_qt;
    int pin_right_qt;
    int count;
    int* angle_arr;
};
