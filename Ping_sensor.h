#include "ping.h"
#include "servo.h"

class Ping_sensor
{
  public:
    Ping_sensor(int pin_s, int pin_p, int number);
    ~Ping_sensor();
    void read();
    void run();//Function to be run on cog with class in shared memory (right?)
    int* distance_arr; //made public for shared memory testing. Should be
                       //private w/ mutator functions
  
  private:
    int pin_servo;
    int pin_ping;
    int count;
    int* angle_arr;
};
