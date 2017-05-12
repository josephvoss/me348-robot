
void ff_funct(int[][6], int[], int[][6]);
/*
 * Modified flood fill. Used to solve directions of the matrix
 *
 * Inputs:
 *      2D array of flood fill values
 *      1D array of goal target location
 *      2D array of walls, in binary notation
 */

int ff_follower(int[], int[], int[][6], int direction, int walls[][6]);
/*
 * Flood Fill Follower? - Takes a filled ff array and returns a position it needs
 * to go
 * 
 * Inputs:
 *      1D array of current location
 *      1D array of goal target location
 *      2D array of flood fill values
 *
 * Outputs:
 *      Integer direction to turn
 */