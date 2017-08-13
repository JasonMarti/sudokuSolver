#ifndef CELL_H
#define CELL_H

using namespace std;
class Cell
{

public:
Cell();
Cell(int newXLoc, int newYLoc, int newDigit, int squareX, int squareY);
~Cell();
void setLocation(int newXloc, int newYLoc, int newSquareX, int newSquareY);
void getLocation(int &currentX, int &currentY, int &currentSquareX, int &currentSquareY);
void setDigit(int newDigit);
void getDigit(int &currentDigit);
void setDomain(int newDomain[9]);
void getDomain(int currentDomain[9]);
void getNumbValInDomain(int &currentNumbvalInDomain);

protected:
int xLoc;
int yLoc;
int digit;
int squareX;
int squareY;
int domain[9];
int numbValInDomain;

};
#endif