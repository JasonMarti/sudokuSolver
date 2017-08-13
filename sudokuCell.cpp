#include "sudokuCell.h"
#include  <vector>
#include <cstddef>
using namespace std;
Cell::Cell()
{
	xLoc = 0;
	yLoc = 0;
	digit = 0; //zero represents empty
	squareX = 0;
	squareY = 0;
	for(int i = 0; i < 9; i++)
	{
		domain[i] = 1;
	}
	numbValInDomain = 9;
}

Cell::Cell(int newXLoc, int newYLoc, int newDigit, int newSquareX, int newSquareY)
{
	xLoc = newXLoc;
	yLoc = newYLoc;
	digit = newDigit;
	squareX = newSquareX;
	squareY = newSquareY;
	if(digit != 0)
	{
		for(int i = 0; i < 9; i++)
		{
			domain[i] = 0;
		}
		numbValInDomain = 0;
	}
	else
	{
		for(int i = 0; i < 9; i++)
		{

			domain[i] = 1;
		}
		numbValInDomain = 9;

	}
}
Cell::~Cell()
{

}

void Cell::setLocation(int newXLoc, int newYLoc, int newSquareX, int newSquareY)
{
	xLoc = newXLoc;
	yLoc = newYLoc;
	squareX = newSquareX;
	squareY = newSquareY;
	return;
}
void Cell::getLocation(int &currentX, int &currentY, int &currentSquareX, int &currentSquareY)
{
	currentX = xLoc;
	currentY = yLoc;
	currentSquareX = squareX;
	currentSquareY = squareY;
	return;
}
void Cell::setDigit(int newDigit)
{
	digit = newDigit;
	for(int i = 0; i < 9; i++)
	{
		domain[i] = 0;
	}
	return;
}
void Cell::getDigit(int &currentDigit)
{
	currentDigit = digit;
	return;
}
void Cell::setDomain(int newDomain[9])
{
	int count = 0;
	for(int i =0; i < 9; i++)
	{
		domain[i] = newDomain[i];
		if(newDomain[i] == 1)
			count++;
	}
	numbValInDomain = count;
}
void Cell::getDomain(int currentDomain[9])
{
	for(int i = 0; i < 9; i++)
	{
		currentDomain[i] = domain[i];
	}

}
void Cell::getNumbValInDomain(int &currentNumbvalInDomain)
{
	currentNumbvalInDomain = numbValInDomain;
}