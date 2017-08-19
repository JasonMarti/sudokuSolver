sudoku : sudokumain.cpp sudokuVariable.o sudokuDriver.o sudokuBoard.o sudokuCell.o sudokuBoard.h sudokuCell.h sudokuDriver.h sudokuVariable.h
	g++ -Wall -g -o sudoku sudokumain.cpp sudokuVariable.o sudokuDriver.o sudokuBoard.o sudokuCell.o
sudokuDriver.o : sudokuDriver.cpp sudokuDriver.h sudokuBoard.h sudokuCell.h sudokuVariable.h
	g++ -c -g sudokuDriver.cpp
sudokuBoard.o : sudokuBoard.cpp sudokuBoard.h sudokuCell.h sudokuVariable.h
	g++ -c -g sudokuBoard.cpp
sudokuCell.o : sudokuCell.cpp sudokuCell.h
	g++ -c -g sudokuCell.cpp
sudokuVariable.o : sudokuVariable.cpp sudokuVariable.h
	g++ -c -g sudokuVariable.cpp
