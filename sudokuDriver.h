#ifndef DRIVER_H
#define DRIVER_H


#include "sudokuBoard.h"
class Driver
{
public:
Driver();
~Driver();
void run();

protected:

//member functions
int debug;
bool generatePuzzle(int userChoice);
Board CSPRecursion(Board, bool& found);
Board constraintSatisfactionStart(Board currentBoard, int newBoard[81]);
void humanPlaying(Board, Board);


//variables
int boardArray[81];

};

#endif