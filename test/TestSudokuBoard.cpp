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

TEST_CASE("Create completed board") {
    SudokuBoard b("126389457738425169549617832374852916892176543651943728983561274417238695265794381");
    REQUIRE(b.isComplete());
    REQUIRE(b.isDone());
}