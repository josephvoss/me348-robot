#ifndef DRIVE_WRAPPER_H
#define DRIVE_WRAPPER_H

class Drive_wrapper
/*
  Drive wrappper class
  Tracks position using dead reckoning from abdrive
  
  Needs to output:
      Current Position
      Position array
          Necessary? Only needed to tie sensor readings and decision points
              Can't hurt for now. Other classes can include pointers to this 
              array to avoid unneccessary copying data
                  If this array is to be accessed by other functions, data 
                  relativity could be an issue.

 */
{
  public:
    Drive_wrapper(int pin_l, int pin_r, int pin_le, int pin_re);
    ~Drive_wrapper();
    void drive(int speed);
    void run(); //Drive cog function. Combine this cog with decision cog?
    void turn_left(void) { turn(1,0); }
    void turn_right(void) { turn(0,1); }
    int get_pos_x(void)  { return cur_pos_x; }
    int get_pos_y(void)  { return cur_pos_y; }
    int get_orientation(void) { return orientation; }
    void set_flag(int* drive_flag) { flag = drive_flag; }
  
  private:
    void turn(int left_turn, int right_turn);
    int cur_pos_x;
    int cur_pos_y;
    int orientation;
    //    0
    //  3   1
    //    2
    int pos_arr_length;
    int** position_array;
    int* flag;
    
};

#endif