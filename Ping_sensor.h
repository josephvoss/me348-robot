#include "ping.h"
#include "servo.h"

class Ping_sensor
{
  public:
    Ping_sensor(int pin_s, int pin_p, int number);
    ~Ping_sensor();
    void read();
    int* distance_arr;
  
  private:
    int pin_servo;
    int pin_ping;
    int count;
    int* angle_arr;
};
