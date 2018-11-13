# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

project(sudoku_stochastic VERSION 0.1.0 LANGUAGES CXX)

add_library(sudoku_stochastic
        GenSudokuBoard.h
        SudokuBoard.h
        constmath.h
        dummy.cpp
        )