
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
    humanPlaying(board, finalBoard);
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
    cout << "\n\nStarting." << endl;
    int x, y, digit, i;
    x = y = digit = -1;
    i = 0;
    char *charPtr;
    bool goodInput;
    string userInput = "";
    while (!humanBoard.isFinished())
    {
        humanBoard.displayBoard();
        while (true)
        {
            cout << "Please enter coordinates and digit as <x> <y> <digit>\n>:";
            getline(cin, userInput);

            while (userInput[i] != '\0')
            {
                if (x < 0 && (userInput[i] <= '9' && userInput[i] >= '0'))
                {
                    x = userInput[i] - '0';
                    i++;
                }
                else if (y < 0 && (userInput[i] <= '9' && userInput[i] >= '0'))
                {
                    y = userInput[i] - '0';
                    i++;
                }
                else if (digit < 0 && (userInput[i] <= '9' && userInput[i] >= '0'))
                {
                    digit = userInput[i] - '0';
                    if (x >= 0 && y >= 0 && digit > 0)
                    {
                        goodInput = true;
                        break;
                    }
                    i++;
                }
                else
                {
                    i++;
                    continue;
                }
            }
            if (!goodInput)
            {
                cout << "not good input " << endl;
            }
            else if (goodInput)
            {
                break;
            }
        }
        goodInput = false;

        cout << "humanBoard.getVal" << endl;
        if (humanBoard.getValueOfCell(x, y) != 0)
            cout << "That spot is already taken" << endl;
        else if (finalBoard.getValueOfCell(x, y) != digit)
        {
            //	cout << "after segfault" << endl;
            cout << "That move eventually violates a constraint." << endl;
            cout << "I reccomend using " << finalBoard.getValueOfCell(x, y) << " there. " << endl;
        }
        else if (finalBoard.getValueOfCell(x, y) == digit && humanBoard.getValueOfCell(x, y) == 0)
            humanBoard.humanBoardUpdate(x, y, digit);

        x = y = digit = -1;
        i = 0;
    }
}
Board Driver::constraintSatisfactionStart(Board currentBoard, int newBoard[81])
{
    bool found = false;
    while (!currentBoard.isFinished() && !found)
    {
        vector<Variable> *MCList = new vector<Variable>;
        vector<Variable> *LCList;
        currentBoard.getMostConstrainedList(MCList);
        int debugx, debugy;
        debugx = debugy = 0;

        while (MCList->size() > 0)
        {
            LCList = new vector<Variable>;
            //getting the list of least constraining values with [0] being the least constraining
            currentBoard.getLeastConstrainingList(MCList->operator[](0), LCList);
            while (LCList->size() > 0)
            {
                //checking the first, least constraining variable, moving to the next if it fails or creating a new board and starting the recoursion if sucessful
                if (currentBoard.forwardChecking(LCList->operator[](0)))
                {
                    currentBoard.getNewBoard(LCList->operator[](0), newBoard);
                    Board updatedBoard(newBoard);
                    updatedBoard.displayBoard();
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
    return currentBoard;
}

Board Driver::CSPRecursion(Board currentBoard, bool &found)
{

    Board finalBoard;
    int newBoard[81];
    debug++;
    if (currentBoard.isFinished())
    {
        return currentBoard;
    }

    else
    {
        vector<Variable> *MCList = new vector<Variable>;
        vector<Variable> *LCList;
        currentBoard.getMostConstrainedList(MCList);
        int x, y, value;
        while (MCList->size() > 0)
        {
            LCList = new vector<Variable>;
            //get the list of least constraining values for the most constrained element
            currentBoard.getLeastConstrainingList(MCList->operator[](0), LCList);
            while (LCList->size() > 0)
            {
                if (currentBoard.forwardChecking(LCList->operator[](0)))
                {
                    LCList->operator[](0).getXY(x, y);
                    LCList->operator[](0).getValue(value);

                    currentBoard.getNewBoard(LCList->operator[](0), newBoard);
                    Board updatedBoard(newBoard);

                    finalBoard = CSPRecursion(updatedBoard, found);
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
                if (LCList->size() > 0)
                    LCList->erase(LCList->begin());
            }
            delete LCList;
            //cout << "backtracking for MCLRoot" << endl;
            if (MCList->size() > 0)
                MCList->erase(MCList->begin());
        }
        if (LCList != NULL)
            delete LCList;
        if (MCList != NULL)
            delete MCList;
    }
    if (!finalBoard.isFinished())
    {
        cout << "unable to find a solution" << endl;
    }
}
