sudoku : sudokumain.cpp sudokuVariable.o sudokuDriver.o sudokuBoard.o sudokuCell.o sudokuBoard.h sudokuCell.h sudokuDriver.h sudokuVariable.h
	g++ -Wall -o sudoku sudokumain.cpp sudokuVariable.o sudokuDriver.o sudokuBoard.o sudokuCell.o
sudokuDriver.o : sudokuDriver.cpp sudokuDriver.h sudokuBoard.h sudokuCell.h sudokuVariable.h
	g++ -c sudokuDriver.cpp
sudokuBoard.o : sudokuBoard.cpp sudokuBoard.h sudokuCell.h sudokuVariable.h
	g++ -c sudokuBoard.cpp
sudokuCell.o : sudokuCell.cpp sudokuCell.h
	g++ -c sudokuCell.cpp
sudokuVariable.o : sudokuVariable.cpp sudokuVariable.h
	g++ -c sudokuVariable.cpp
