
#include "sudokuCell.h"
#include "sudokuBoard.h"
#include "sudokuVariable.h"
#include <iostream>
#include <algorithm>
#include <cstddef>
using namespace std;


Board::Board()
{
	//doesn't need to do anything
}

Board::Board(int board[81])
{
	finished = false;
	//setting the board values and any domains that are empty becasue of assignment
	int tempIter;
	int tempSqX;
	int tempSqY;
	int emptyDomain[9];

	for (int i = 0; i < 9; i++)
	{
		emptyDomain[i] = 0;
	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			tempIter = i * 9 + j;
			tempSqX = j / 3;
			tempSqY = i / 3;
			if (board[tempIter] != 0)
			{
				cells[j][i].setDigit(board[tempIter]);
				cells[j][i].setLocation(j, i, tempSqX, tempSqY);
				cells[j][i].setDomain(emptyDomain);
			}
			else
			{
				cells[j][i].setLocation(j, i, tempSqX, tempSqY);
			}
		}
	}
	//setting the rest of the domains to reflect what they should be
	int currentX, currentY, currentSqX, currentSqY, currentDigit, currentNumbInDomain = 0;
	int offsetX, offsetY = 0;

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cells[j][i].getDigit(currentDigit);
			if (currentDigit != 0)
			{
				cells[j][i].getLocation(currentX, currentY, currentSqX, currentSqY);
				for (int m = 0; m < 9; m++) //for peers along the x axis
				{
					cells[m][currentY].getDomain(emptyDomain);
					if (emptyDomain[currentDigit - 1] == 1)
					{
						emptyDomain[currentDigit - 1] = 0;
					}
					cells[m][currentY].setDomain(emptyDomain);
				}

				for (int m = 0; m < 9; m++) //for peers along the y axis
				{
					cells[currentX][m].getDomain(emptyDomain);
					if (emptyDomain[currentDigit - 1] == 1)
					{
						emptyDomain[currentDigit - 1] = 0;
					}
					cells[currentX][m].setDomain(emptyDomain);
				}

				offsetX = currentSqX * 3;
				offsetY = currentSqY * 3;
				for (int m = 0; m < 3; m++) //for peers in the square
				{
					for (int n = 0; n < 3; n++)
					{
						cells[offsetX + n][offsetY + m].getDomain(emptyDomain);
						if (emptyDomain[currentDigit - 1] == 1)
						{
							emptyDomain[currentDigit - 1] = 0;
						}
						cells[offsetX + n][offsetY + m].setDomain(emptyDomain);
					}
				}
			}
		}
	}
}

void Board::getMostConstrainedList(vector<Variable> *MCList)
{
	//misc support variables
	Variable varInserter;
	int digit = 0;
	int constrained = 0;
	int domain[9];
	int debugCount = 0;
	int debugX, debugY = 0;

	//setting up variable for sort function
	bool (*comp)(Variable, Variable);
	comp = &Board::compareVariables;

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			//taking each cell in the board, checks if is occupied, if not adds it to MCList
			cells[j][i].getDigit(digit);
			if (digit == 0)
			{
				//checks domain to find how constrained
				cells[j][i].getDomain(domain);
				//most constrained variable has the lowest constrained value
				for (int m = 0; m < 9; m++)
				{
					if (domain[m] == 1) //each cell has a 1 in the cooresponding index, eg [4] cooresponds to 5, each value that has a 1 is constrained by something
						constrained++;
				}
				//setting the values on the object for insertion into the vector
				varInserter.setConstrained(constrained);
				varInserter.setXY(j, i);

				//inserting variable into the unsorted list
				MCList->push_back(varInserter);

				//reinitlize constrained for next loop
				constrained = 0;
			}
		}
	}


	//inserting finished begin sort;
 	
	sort(MCList->begin(), MCList->end(), comp); 

}
void Board::getLeastConstrainingList(Variable MCLVar, vector<Variable> *LCList)
{
	//misc supporting variables
	int x, y, digit = 0;
	int domain[9];
	int tempDomain[9];
	int constrained[9];
	for (int i = 0; i < 9; i++)
	{
		constrained[i] = 0;
	}

	MCLVar.getXY(x, y);
	cells[x][y].getDomain(domain); //pulling domain to get potential digits
	for (int i = 0; i < 9; i++)
	{
		//collecting constraints on each possible digit
		//gets the domain for the cell, then adds a constraint tally if the value is present in the cell
		//checking along the y axis
		cells[x][i].getDomain(tempDomain);
		for (int j = 0; j < 9; j++)
		{
			if (tempDomain[j] == 1 && i != y) //excludes the original cell, 1 means the digit is in the domain,
				constrained[j]++;			  //if the digit is in the domain of a peer it makes setting it in the original cell more constraining so increment
		}
		//checking along the x axis
		cells[i][y].getDomain(tempDomain);
		for (int j = 0; j < 9; j++)
		{
			if (tempDomain[j] == 1 && i != x) //excludes the original cell, 1 means the digit is in the domain
				constrained[j]++;			  //if the digit is in the domain of a peer it makes setting it in the original cell more constraining so increment
		}
	}
	//checking peers in the same 3x3 grid
	int offsetY, offsetX = 0;
	offsetX = x / 3; //offset to find the location of the top left corner x
	offsetY = y / 3; //offset to find the location of the top left corner y
	for (int i = 0; i < 3; i++)
	{

		for (int j = 0; j < 3; j++)
		{
			cells[offsetX * 3 + j][offsetY * 3 + i].getDomain(tempDomain);
			for (int m = 0; m < 9; m++)
			{
				if (tempDomain[m] == 1)
				{
					if ((offsetX * 3 + j) != x && (offsetY * 3 + i) != y) //if neither is equal to original cell, count it
					{
						constrained[m]++;
					}
					else if ((offsetX * 3 + j) == x && (offsetY * 3 + i) != y) //if x is equal but y is not, count it
					{
						constrained[m]++;
					}
					else if ((offsetX * 3 + j) != x && (offsetY * 3 + i) == y) //if y is equal but x is not, count it
					{
						constrained[m]++;
					}
				}
			}
		}
	}
	Variable varInserter;
	for (int i = 0; i < 9; i++)
	{
		if (domain[i] == 1) //for each digit in the domain of the original cell it populates the constraints in the variable
		{
			varInserter.setValue(i); //value starts at 0 where digits start at 1
			varInserter.setConstrained(constrained[i]);
			varInserter.setXY(x,y);
			LCList->push_back(varInserter);
		}
	}

	return;
}
bool Board::forwardChecking(Variable LCLVar)
{
	//misc supporting variables
	int numInDomain, digit, x, y, offsetX, offsetY;
	int domain[9];

	//initalizing supporting variables
	numInDomain = digit = x = y = offsetX = offsetY = 0;
	for (int i = 0; i < 9; i++)
	{
		domain[i] = 0;
	}
	LCLVar.getXY(x, y);
	LCLVar.getValue(digit);
	digit++;
	offsetX = x / 3;
	offsetY = y / 3;

	//checking vertical peers
	for (int i = 0; i < 9; i++)
	{
		//get the domain for each cell in the vertical peers
		cells[x][i].getDomain(domain);
		if (i == y) //skip original cell
		{
			continue;
		}

		//check if the domain of the cell includes the digit to be set
		if (domain[digit - 1] == 1)
		{
			//if the domain includes it check the rest of the domain to see if it is the only item in the domain
			for (int m = 0; m < 9; m++)
			{
				if (domain[m] == 1) //if there is something in the domain increment numInDomain
				{

					numInDomain++;
					if (numInDomain > 1) //if there is at least one other digit in the domain, no need to keep checking the rest
					{
						break;
					}
				}
			}

			if (numInDomain == 1) //if there is only the digit in the domain forward checking fails and returns false
			{
				return false;
			}
		}
	}
	//reset for next set of peers
	numInDomain = 0;
	//checking horizontal peers
	for (int i = 0; i < 9; i++)
	{
		//get the domain for each horizontal peer
		cells[i][y].getDomain(domain);
		if (i == x) //ignore the original
		{
			continue;
		}

		//check if the domain includes the digit to be set
		if (domain[digit - 1] == 1)
		{
			for (int m = 0; m < 9; m++) //if any cells have the digit in the domain check if there are other digits in the domain
			{
				if (domain[m] == 1) //increment numInDomain for each digit in the domain
				{
					numInDomain++;
					if (numInDomain > 1) //if there is at least one other digit other than the one checking no need to continue checking
					{
						break;
					}
				}
			}
			if (numInDomain == 1) //if there is only the digit in the domain forward checking fails and returns false
			{
				return false;
			}
		}
	}

	//reset for next set of peers
	numInDomain = 0;
	//checking peers in the 3x3 square
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//collecting domain for each cell
			cells[offsetX * 3 + j][offsetY * 3 + i].getDomain(domain);


			if (((offsetX * 3) + j) == x && ((offsetY * 3) + i) == y) //ignore the original
				continue;
			if (domain[digit - 1] == 1)
			{
				for (int m = 0; m < 9; m++)  //if any cells have the digit in the domain check if there are other digits in the domain
				{
					if (domain[m] == 1)		 //increment numInDomain for each digit in the domain
					{
						numInDomain++;
						if (numInDomain > 1) //if there is at least one other digit other than the one checking no need to continue checking
							break;
					}
				}
				if (numInDomain == 1)		 //if there is only the digit in the domain forward checking fails and returns false 
				{

					return false;
				}
			}
		}
	}

	//after checking all peers and passing, returns true
	return true;
}
void Board::getNewBoard(Variable NextMove, int newBoard[81])
{
	//misc support variables
	int digit = 0;
	int x, y = 0;

	//get location of the digit to be changed
	NextMove.getXY(x, y);

	//use a for loop to populate an array that represents the current board
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cells[j][i].getDigit(digit);
			newBoard[i * 9 + j] = digit;
		}

	}
	//get the digit to be changed then change it
	NextMove.getValue(digit);
	digit++; //to correct for the difference between value and number on the board
	newBoard[y * 9 + x] = digit;

	return; //newboard array is populated with the updated sudoku board
}
bool Board::isFinished()
{
	int digit = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cells[j][i].getDigit(digit);
			if (digit == 0)
				return false;
		}
	}
	int digitsX[9];
	int digitsY[9];
	int count = 0;
	for (int i = 0; i < 9; i++)
	{
		digitsX[i] = 1;
		digitsY[i] = 1;
	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cells[j][i].getDigit(digit);
			if (digitsX[digit - 1] == 0)
				return false;
			else
				digitsX[digit - 1] = 0;
			cells[i][j].getDigit(digit);
			if (digitsY[digit - 1] == 0)
				return false;
			else
				digitsY[digit - 1] = 0;
		}
		for (int n = 0; n < 9; n++)
		{
			if (digitsX[n] == 1 || digitsY[n] == 1)
				return false;
			else
			{
				digitsX[n] = 1;
				digitsY[n] = 1;
			}
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int m = 0; m < 3; m++)
			{
				for (int n = 0; n < 3; n++)
				{
					cells[j * 3 + n][i * 3 + m].getDigit(digit);
					if (digitsX[digit - 1] == 0)
						return false;
					else
						digitsX[digit - 1] = 0;
				}
			}
			for (int m = 0; m < 9; m++)
			{
				if (digitsX[m] == 1)
					return false;
				else
					digitsX[m] = 1;
			}
		}
	}
	return true;
}
void Board::displayBoard()
{
	int tempdigit = 0;
	cout << "   0 1 2 3 4 5 6 7 8" << endl;
	for (int i = 0; i < 9; i++)
	{
		cout << i << " |";
		for (int j = 0; j < 9; j++)
		{
			cells[j][i].getDigit(tempdigit);
			if (tempdigit != 0)
				cout << tempdigit << "|";
			else if (tempdigit == 0)
				cout << " |";
		}
		cout << endl;
	}
}
int Board::countZeros()
{
	int digit, count = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cells[j][i].getDigit(digit);
			if (digit == 0)
				count++;
		}
	}
	return count;
}

void Board::humanBoardUpdate(int x, int y, int digit)
{
	cells[x][y].setDigit(digit);
}
int Board::getValueOfCell(int x, int y)
{
	int digit;
	cells[x][y].getDigit(digit);
	return digit;
}

bool Board::compareVariables(Variable var1, Variable var2)
{
	if (var1.getConstrained() < var2.getConstrained())
	{
	  return true;
	}
	else
	{
	  return false;
	}
  
}
