#include "sudokuVariable.h"
#include <cstddef>
using namespace std;

Variable::Variable()
{
	x = 0;
	y = 0;
	value = 0;
	constrainedAmount = 9;
	//TODO
}
Variable::~Variable()
{
	//TODO
}

void Variable::setXY(int newX, int newY)
{
	x = newX;
	y = newY;
}
void Variable::getXY(int &currentX, int &currentY)
{
	currentX = x;
	currentY = y;
}
void Variable::setValue(int newValue)
{
	value = newValue;
}
void Variable::getValue(int &currentValue)
{
	currentValue = value;
}
void Variable::setConstrained(int newConstrained)
{
	constrainedAmount = newConstrained;
}
int Variable::getConstrained()
{
	return constrainedAmount;
}