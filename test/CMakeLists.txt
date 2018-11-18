# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

cmake_minimum_required(VERSION 3.12)
project(sudoku_stochastic_test)

# Enables testing from this directory downwards.
enable_testing(true)

# Prepare "Catch" library for other executables.
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make main executable that will be linked with other tests.
set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests Catch)

add_executable(TestSudokuBoard TestSudokuBoard.cpp ${TEST_SOURCES})
target_link_libraries(TestSudokuBoard Catch stochastic OpenMP::OpenMP_CXX)

add_executable(TestSudokuBoardPopulator TestSudokuBoardPopulator.cpp ${TEST_SOURCES})
target_link_libraries(TestSudokuBoardPopulator.cpp Catch stochastic OpenMP::OpenMP_CXX)

add_executable(TestPopulationTournamentSelection TestPopulationTournamentSelection.cpp ${TEST_SOURCES})
target_link_libraries(TestPopulationTournamentSelection Catch stochastic OpenMP::OpenMP_CXX)

add_executable(TestGeneticSudokuBoardSolver TestGeneticSudokuBoardSolver.cpp ${TEST_SOURCES})
target_link_libraries(TestGeneticSudokuBoardSolver Catch stochastic OpenMP::OpenMP_CXX)
