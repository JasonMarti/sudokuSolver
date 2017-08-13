
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>

#include "sudokuCell.h"
#include "sudokuBoard.h"
#include "sudokuVariable.h"
#include "sudokuDriver.h"

/*TODO
move the file transfer to the pullPuzzleFromFile [done]
make it void and only take input from the user [done]
sanatize input from the user for the puzzle choice/choice for cell input [inc]
change the linked lists to vectors: change all variables to vectors

*/

/*function prototypes
Board CSPRecursion(Board, bool& found);
Board constraintSatisfactionStart(Board currentBoard, int newBoard[81]);
void humanPlaying(Board, Board);
void destroy(Variable*);
*/

Driver::Driver()
{
    for (int i = 0; i < 81; i++)
    {
        boardArray[i] = 0;
    }
}
Driver::~Driver()
{
}

void Driver::run()
{

    int difficutly = 0;
    string rawInput;
    int count = 0;
    int i = 0;

    //User Introduction
    cout << "welcome, please chose a puzzle diffuclty from 0 to 3: " << endl;
    
    //pulling input from the user
    getline(cin, rawInput);
    difficutly = rawInput[0] - '0';
    generatePuzzle(difficutly);

    //display final board
    cout << "continuing with this board: " << endl;
    Board board(boardArray);
    board.displayBoard();
    cout << endl
         << endl;
    Board finalBoard;

    //find correct board positions
    finalBoard = constraintSatisfactionStart(board, boardArray);

    humanPlaying(board, finalBoard);
}

void Driver::generatePuzzle(int userChoice)
{
    //TODO add file transfer things
    ifstream fin;
    int count = 0;

    fin.open("puzzles.txt");

    if(!ios_base::failbit)
    {
        cout << "unable to find puzzles.txt" << endl;
    } 
    int i = 0;
    int j = 0;
    char fChar = 'a';
    while (!fin.eof())
    {
        j++;
        fin.get(fChar);
        if (count > userChoice)
        {
            break;
        }
        else if (fChar == '\n')
        {
            
            cout << "count: " << count << endl; 
            count++;
            continue;
        }
        else if (count == userChoice)
        {
            boardArray[i] = fChar - '0';
            i++;
        }
    }
    cout << "j: " << j << endl;
}

void Driver::humanPlaying(Board humanBoard, Board finalBoard)
{
    cout << "Begin." << endl;
    int x, y, digit = -1;
    char input, input1, input2;
    while (!humanBoard.isFinished())
    {
        humanBoard.displayBoard();
        cout << "please enter your choice in the form <x> <enter> <y> <enter> <digit> <enter>" << endl;
        cin >> input;
        cin >> input1;
        cin >> input2;
        x = input - '0';
        y = input1 - '0';
        digit = input2 - '0';
        //cout << "before segfault" << endl;
        if (finalBoard.getValueOfCell(x, y) != digit)
        {
            //	cout << "after segfault" << endl;
            cout << "That move eventually violates a constraint." << endl;
            cout << "I reccomend using " << finalBoard.getValueOfCell(x, y) << " there. " << endl;
        }
        else if (finalBoard.getValueOfCell(x, y) == digit && humanBoard.getValueOfCell(x, y) == 0)
            humanBoard.humanBoardUpdate(x, y, digit);
        else if (humanBoard.getValueOfCell(x, y) != 0)
            cout << "That spot is already taken" << endl;
    }
}
Board Driver::constraintSatisfactionStart(Board currentBoard, int newBoard[81])
{
    bool found = false;
    while (!currentBoard.isFinished())
    {
        //DEBUG COMMENT REMOVE ACTIVE
        cout << "in while loop for constraintSatisfaction" << endl;
        vector<Variable> MCList;
        vector<Variable> LCList;
        currentBoard.getMostConstrainedList();
        //DEBUG COMMENT REMOVE ACTIVE
        cout << "after getMostConstrainedList" << endl;
        while (MCList.size() > 0)
        {
            LCLRoot = currentBoard.getLeastConstrainingList(MCLRoot);
            while (LCLRoot != NULL)
            {

                if (currentBoard.forwardChecking(LCLRoot))
                {
                    currentBoard.getNewBoard(LCLRoot, newBoard);
                    Board updatedBoard(newBoard);
                    Board finalBoard = CSPRecursion(updatedBoard, found);
                    if (finalBoard.isFinished())
                    {
                        // cout << "returns" << endl;
                        // destroy(LCLRoot);
                        // destroy(MCLRoot);
                        return finalBoard;
                    }
                }
                else
                    LCLRoot = LCLRoot->getNext();
            }

            MCLRoot = MCLRoot->getNext();

        }
    }
    cout << "A solution wasn't found" << endl;
}

Board Driver::CSPRecursion(Board currentBoard, bool &found)
{
    Board finalBoard;
    //cout << "does it get to here" << endl;
    int newBoard[81];
    if (found == true || currentBoard.isFinished())
    {
        //cout <<"did it make it this far" << endl;
        //found = true;
        return currentBoard;
    }

    else
    {
        Variable *MCLRoot;
        Variable *LCLRoot;
        MCLRoot = currentBoard.getMostConstrainedList();
        Variable *test;
        test = MCLRoot;
        int x, y, value;
        /*while(test != NULL)
			{
				test->getXY(x, y);
				cout << "X: " << x << " Y: " << y << endl;
				test = test->getNext();
			}*/

        while (MCLRoot != NULL)
        {
            //cout << "testing" << endl;
            //get the list of least constraining values for the most constrained element
            LCLRoot = currentBoard.getLeastConstrainingList(MCLRoot);
            while (LCLRoot != NULL)
            {
                //cout << "2 ";

                if (currentBoard.forwardChecking(LCLRoot))
                {
                    LCLRoot->getXY(x, y);
                    LCLRoot->getValue(value);

                    //c//out << "testing3" << endl;
                    //cout << LCLRoot-> getConstrained() << endl;
                    currentBoard.getNewBoard(LCLRoot, newBoard);
                    //cout << "testing5" << endl;
                    /*cout << "<what is in newBoard> " << endl;
					for(int i = 0; i < 81; i++)
					{
						cout << newBoard[i] << " ";
						if(i %9 == 0)
							cout << endl;
					}
					cout << "<what is in newBoard/>" << endl;*/

                    Board updatedBoard(newBoard);
                    //cout << "testing6" << endl;
                    //cout << "bool" << found << endl;
                    /*cout << endl;
					updatedBoard.displayBoard();
					cout << endl;*/
                    finalBoard = CSPRecursion(updatedBoard, found);
                    //cout << "testing7" << endl;
                    if (finalBoard.isFinished())
                    {
                        Variable *tnt = new Variable;
                        // if(LCLRoot == NULL)
                        // 	cout << "is NULL" << endl;
                        // if(LCLRoot != NULL)
                        // 	destroy(LCLRoot);
                        // if(MCLRoot != NULL)
                        // 	destroy(MCLRoot);
                        return finalBoard;
                    }
                }
                //cout <<"backstracking within a node" << endl;
                LCLRoot = LCLRoot->getNext();
            }
            //cout << "backtracking for MCLRoot" << endl;
            MCLRoot = MCLRoot->getNext();
        }
    }
}

void Driver::destroy(Variable *element)
{
    Variable *forwardT;
    Variable *forwardD;
    Variable *backwardT;
    Variable *backwardD;
    if (element->getNext() != NULL)
    {
        forwardT = element->getNext();
        while (forwardT->getNext() != NULL)
        {
            forwardD = forwardT;
            forwardT = forwardT->getNext();
            delete forwardD;
        }
        delete forwardT;
    }
    if (element->getPrev() != NULL)
    {
        backwardT = element->getPrev();
        while (backwardT->getPrev() != NULL)
        {
            backwardD = backwardT;
            backwardT = backwardT->getPrev();
            delete backwardD;
        }
        delete backwardT;
    }
    delete element;
}
