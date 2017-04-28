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

   TO DO:
   Calibration
       Needs to be run once, output data stored in EEPROM

 */
{
  public:
    Drive_wrapper(int pin_l, int pin_r, int log_length);
    ~Drive_wrapper();
    void drive(int left_turn, int right_turn);
    void run(); //Drive cog function. Combine this cog with decision cog?
  
  private:
    int pin_left;
    int pin_right;
    int cur_pos_x;
    int cur_pos_y;
    int orientation;
    //    0
    //  3   1
    //    2
    int pos_arr_length;
    int** position_array;
    
};
