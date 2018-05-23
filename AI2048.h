#ifndef AI2048_H
#define AI2048_H

#include "2048.h"
#define MAX 9999
#define MIN -9999

// Adjustable Parameters
#define DEPTH 4
#define TESTTIMES 100
#define BLANK 7
#define SMOOTH 2
#define MONO 8
#define CORNER 1


/**
 * @brief Calculate result of log_2
 * 
 * @param Value input value
 * @return int number of powers
 */
int Log2Value( int Value );

/**
 * @brief Search for best direction
 * 
 * @param Address Grid address
 * @param Depth Search depth
 * @return dir_e Direction enum
 */
dir_e Search( Grid& Address, int* Depth );

/**
 * @brief Search for maximum Heuristic value
 * 
 * @param Address Grid address
 * @param Depth Search depth 
 * @return int Maximum heuristic value
 */
int SearchDirection( Grid& Address, int Depth );

/**
 * @brief Search for expected value of Heuristic value
 * 
 * @param Address Grid address
 * @param Depth Search depth 
 * @return int Expected value of heuristic value
 */
int SearchAppreance( Grid& Address, int Depth );

/**
 * @brief Determine if game is over
 * 
 * @param Address Grid address
 * @return true Game is over
 * @return false Game is not over
 */
bool isGameOver( Grid& Address );

/**
 * @brief Evaluate the Grid
 * 
 * @param Address Grid address
 * @return int Heuristic value
 */
int Evaluate( Grid& Address );

/**
 * @brief Evaluate smoothness 
 * 
 * @param Address Grid address 
 * @return int Heuristic value of somoothness
 */
int Smoothness( Grid& Address );

/**
 * @brief Evaluate monotonicity
 * 
 * @param Address Grid address 
 * @return int Heuristic value of monotonicity
 */
int Monotonicity( Grid& Address );

/**
 * @brief Evalue corner property
 * 
 * @param Address Grid address
 * @return int Heuristic value of corner
 */
int Corner( Grid& Address );
#endif