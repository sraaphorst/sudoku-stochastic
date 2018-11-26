# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

add_executable(sudoku_ga sudoku_ga.cpp)
target_link_libraries(sudoku_ga PRIVATE stochastic ${Boost_PROGRAM_OPTIONS_LIBRARY} OpenMP::OpenMP_CXX)

add_executable(sudoku_hc sudoku_hc.cpp)
target_link_libraries(sudoku_hc PRIVATE stochastic OpenMP::OpenMP_CXX)

add_executable(sudoku_bhc sudoku_bhc.cpp)
target_link_libraries(sudoku_bhc PRIVATE stochastic OpenMP::OpenMP_CXX)

add_executable(sudoku_gd sudoku_gd.cpp)
target_link_libraries(sudoku_gd PRIVATE stochastic OpenMP::OpenMP_CXX)