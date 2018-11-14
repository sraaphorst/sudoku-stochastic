/**
 * TestSudokuBoard.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>
#include <string_view>
#include "SudokuBoard.h"

using namespace sudoku_stochastic;

TEST_CASE("Create from string") {
    SudokuBoard b{"100089457738000000040010000004050906000000000000000728080001000007008095060090300"};
    REQUIRE(!b.isComplete());
}

TEST_CASE("Test completed board") {
    SudokuBoard b("126389457738425169549617832374852916892176543651943728983561274417238695265794381");
    REQUIRE(b.isComplete());
    REQUIRE(b.isDone());
}

TEST_CASE("Test error counting") {
    // Errors in this board: 6
    // 1 error:  two 9s in row 0
    // 3 errors: three 1s in col 0
    // 1 error:  two 1s in grid (0,0)
    // 1 error:  two 9s in grid (1,2)
    SudokuBoard b("102987459"
                  "134652798"
                  "785134260"
                  "100000000"
                  "000000000"
                  "000000000"
                  "000000000"
                  "000000000"
                  "000000000");
    REQUIRE(!b.isComplete());
    REQUIRE(b.findNumberOfErrors() == 6);
}