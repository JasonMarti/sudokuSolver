sudoku : sudokumain.o sudokuVariable.o sudokuDriver.o sudokuBoard.o sudokuCell.o 
	g++ -Wall -o sudoku sudokumain.o sudokuVariable.o sudokuDriver.o sudokuBoard.o sudokuCell.o
sudokumain.o : sudokumain.cpp sudokuDriver.h
	g++ sudokumain.cpp sudokuDriver.cpp
sudokuDriver.o : sudokuDriver.cpp sudokuDriver.h sudokuBoard.h sudokuCell.h sudokuVariable.h
	g++ sudokuDriver.cpp
sudokuBoard.o : sudokuBoard.cpp sudokuBoard.h sudokuCell.h sudokuVariable.h
	g++ sudokuBoard.cpp
sudokuCell.o : sudokuCell.cpp sudokuCell.h
	g++ sudokuCell.cpp
sudokuVariable.o : sudokuVariable.cpp sudokuVariable.h
	g++ sudokuVariable.cpp
