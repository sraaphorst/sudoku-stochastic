# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

add_executable(sudoku_ga sudoku_ga.cpp)
target_link_libraries(sudoku_ga PRIVATE stochastic ${Boost_PROGRAM_OPTIONS_LIBRARY} OpenMP::OpenMP_CXX)

set(HEURISTICS
        sudoku_hc
        sudoku_bhc
        sudoku_gd
        sudoku_sa
        sudoku_ts
)

foreach (heuristic ${HEURISTICS})
    add_executable(${heuristic} ${heuristic}.cpp)
    target_link_libraries(${heuristic} PRIVATE stochastic ${Boost_PROGRAM_OPTIONS_LIBRARY})
endforeach()
