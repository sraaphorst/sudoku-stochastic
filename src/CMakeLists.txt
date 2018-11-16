# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

project(sudoku_stochastic VERSION 0.1.0 LANGUAGES CXX)

add_library(sudoku_stochastic SHARED
        GenSudokuBoard.h
        GenSudokuBoardFiller.h
        PopulationSelector.h
        SudokuBoard.h
        SudokuBoardFiller.h
        constmath.h
        RNG.h
        RNG.cpp
        )