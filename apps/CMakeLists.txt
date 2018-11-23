# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

add_executable(sudoku_ga sudoku_ga.cpp)
target_link_libraries(sudoku_ga PRIVATE stochastic OpenMP::OpenMP_CXX)

add_executable(sudoku_hc sudoku_hc.cpp)
target_link_libraries(sudoku_hc PRIVATE stochastic OpenMP::OpenMP_CXX)