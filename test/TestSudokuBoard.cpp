/**
 * TestSudokuBoard.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <algorithm>
#include <iterator>
#include <set>
#include <string_view>
#include <tuple>
#include <unordered_set>

#include "GenSudokuBoard.h"

using namespace vorpal::gensudoku;

TEST_CASE("Create from string") {
    SudokuBoard b{"100089457738000000040010000004050906000000000000000728080001000007008095060090300"};
    REQUIRE(!b.isFull());
}

TEST_CASE("Test completed board") {
    SudokuBoard b{"126389457738425169549617832374852916892176543651943728983561274417238695265794381"};
    REQUIRE(b.isFull());
    REQUIRE(b.isDone());
    REQUIRE(b.fitness() == SudokuBoard::PerfectFitness);
}

TEST_CASE("Test error counting") {
    SudokuBoard b{
           "876591234"
           "572463918"
           "574281639"
           "745819623"
           "792863541"
           "368714295"
           "694527138"
           "512936847"
           "327641985"
    };
    REQUIRE(b.isFull());
    REQUIRE(b.fitness() == 203);
}

TEST_CASE("Test hasValidEntries") {
    // A base-17 board with valid entries.
    GenSudokuBoard<4> b {
        "123456789abcdefg"
        "23456789abcdefg1"
        "3456789abcdefg12"
        "456789abcdefg123"
        "56789abcdefg1234"
        "6789abcdefg12345"
        "789abcdefg123456"
        "89abcdefg1234567"
        "9abcdefg12345678"
        "abcdefg123456789"
        "bcdefg123456789a"
        "cdefg123456789ab"
        "defg123456789abc"
        "efg123456789abcd"
        "fg123456789abcde"
        "g123456789abcdef"
    };
    REQUIRE(b.hasValidEntries());

    // A regular Sudoku but over base 11. Womp womp.
    constexpr SudokuBoard b2 {
        "123456789"
        "a12345678"
        "9a1234567"
        "89a123456"
        "789a12345"
        "6789a1234"
        "56789a123"
        "456789a12"
        "34567889a"
    };
    REQUIRE(!b2.hasValidEntries());
}
