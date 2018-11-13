# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

add_executable(sudoku_genetic sudoku_genetic.cpp)
target_link_libraries(sudoku_genetic PRIVATE sudoku_stochastic OpenMP::OpenMP_CXX)