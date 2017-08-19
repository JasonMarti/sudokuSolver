
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
    debug = 0;
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

    int difficulty = 0;
    string rawInput;
    int count = 0;
    int i = 0;

    //User Introduction
    cout << "welcome, please chose a puzzle diffuclty from 0 to 3: " << endl;

    //pulling input from the user
    getline(cin, rawInput);
    difficulty = atoi(rawInput.c_str());
    if (difficulty > 3 || difficulty < 0)
    {
        cout << "please enter a digit between 0 and 3 inclusive.\n:";
    }
    //giving difficulty to the generate function.
    if (!generatePuzzle(difficulty))
    {
        return;
    }

    //display final board
    cout << "continuing with this board: " << endl;
    Board board(boardArray);
    board.displayBoard();
    cout << endl
         << endl;
    Board finalBoard;

    //find correct board positions
    finalBoard = constraintSatisfactionStart(board, boardArray);
    finalBoard.displayBoard();
    //humanPlaying(board, finalBoard);
}

bool Driver::generatePuzzle(int userChoice)
{
    //TODO add comments
    ifstream fin;
    int count = 0;

    fin.open("puzzles.txt");

    if (fin.fail())
    {
        cout << "unable to find puzzles.txt" << endl;
        return false;
    }
    int i = 0;
    string fileInput;
    while (!fin.eof())
    {
        getline(fin, fileInput);
        cout << "fileInput '" << fileInput << "'" << endl; 
        if (i == userChoice)
        {
            for (int m = 0; m < 81; m++)
            {
                boardArray[m] = (fileInput[m] - '0');
            }
            break;
        }
        else
        {
            i++;
        }
    }
    return true;
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
    while (!currentBoard.isFinished() && !found)
    {
        //DEBUG COMMENT REMOVE ACTIVE
        cout << "in while loop for constraintSatisfaction" << endl;
        vector<Variable> *MCList = new vector<Variable>;
        vector<Variable> *LCList;
        currentBoard.getMostConstrainedList(MCList);
        //DEBUG COMMENT REMOVE ACTIVE
        cout << "after getMostConstrainedList" << endl;
        int debugx, debugy;
        debugx = debugy = 0;
        cout << "MCList->size()" << MCList->size();
        cout << endl;
        MCList->operator[](0).getXY(debugx, debugy);
        cout << "x" << debugx << "y" << debugy << " | ";
        cout << endl;
        for (unsigned int i = 0; i < MCList->size(); i++)
        {
            MCList->operator[](i).getXY(debugx, debugy);
            cout << "x" << debugx << "y" << debugy << " | ";
            if((i+1) % 9 == 0)
            cout << endl;
        }
        while (MCList->size() > 0)
        {
            LCList = new vector<Variable>;
            //getting the list of least constraining values with [0] being the least constraining
            currentBoard.getLeastConstrainingList(MCList->operator[](0), LCList);
            while (LCList->size() > 0)
            {
                for (unsigned int i = 0; i < LCList->size(); i++)
                {
                    LCList->operator[](i).getValue(debugx);
                    cout << "value" << debugx << endl;
                }
                //checking the first, least constraining variable, moving to the next if it fails or creating a new board and starting the recoursion if sucessful
                if (currentBoard.forwardChecking(LCList->operator[](0)))
                {
                    cout << "forward checking passed" << endl;
                    currentBoard.getNewBoard(LCList->operator[](0), newBoard);
                    Board updatedBoard(newBoard);
                    Board finalBoard = CSPRecursion(updatedBoard, found);
                    if (finalBoard.isFinished() || found)
                    {
                        return finalBoard;
                    }
                }
                else
                    //erase the first index
                    LCList->erase(LCList->begin());
            }
            delete LCList;
            //erase the first index
            MCList->erase(MCList->begin());
        }
        delete LCList;
        delete MCList;
    }
    cout << "A solution wasn't found" << endl;
}

Board Driver::CSPRecursion(Board currentBoard, bool &found)
{
    cout << "\nonce around the merry-go round" << endl;
    Board finalBoard;
    //cout << "does it get to here" << endl;
    int newBoard[81];

    cout << "debug: " << debug << endl;
    debug++;
    if (found == true || currentBoard.isFinished())
    {
        cout << "did it make it this far" << endl;
        //found = true;
        return currentBoard;
    }

    else
    {
        vector<Variable> *MCList = new vector<Variable>;
        vector<Variable> *LCList;
        cout << "before getMostConstrainedList" << endl;
        currentBoard.displayBoard();
        currentBoard.getMostConstrainedList(MCList);
        int x, y, value;
        /*while(test != NULL)
			{
				test->getXY(x, y);
				cout << "X: " << x << " Y: " << y << endl;
				test = test->getNext();
			}*/

        while (MCList->size() > 0)
        {
            LCList = new vector<Variable>;
            cout << "after getMostConstrainedList" << endl;
            //get the list of least constraining values for the most constrained element
            currentBoard.getLeastConstrainingList(MCList->operator[](0), LCList);
            cout << "after getLeastConstraingList" << endl;
            while (LCList->size() > 0)
            {
                cout << "before forward checking" << endl;
                if (currentBoard.forwardChecking(LCList->operator[](0)))
                {
                    cout << "after forward checking" << endl;
                    LCList->operator[](0).getXY(x, y);
                    LCList->operator[](0).getValue(value);

                    cout << "after LCList->operator" << endl;
                    currentBoard.getNewBoard(LCList->operator[](0), newBoard);
                    Board updatedBoard(newBoard);
                    cout << "after updating board" << endl;

                    //finalBoard = CSPRecursion(updatedBoard, found);
                    found = true;
                    return updatedBoard;
                    cout << "after updating board" << endl;
                    if (finalBoard.isFinished())
                    {
                        found = true;
                        return finalBoard;
                    }
                    else
                    {
                        cout << "after finalboard.isFinished() returns false" << endl;
                    }
                }
                //cout <<"backstracking within a node" << endl;
                LCList->erase(LCList->begin());
            }
            delete LCList;
            //cout << "backtracking for MCLRoot" << endl;
            MCList->erase(MCList->begin());
        }
        if (LCList != NULL)
            delete LCList;
        delete MCList;
    }
}
