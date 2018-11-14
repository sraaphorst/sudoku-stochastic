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

#include "SudokuBoard.h"

using namespace sudoku_stochastic;

TEST_CASE("Create from string") {
    constexpr SudokuBoard b{"100089457738000000040010000004050906000000000000000728080001000007008095060090300"};
    REQUIRE(!b.isFull());
}

TEST_CASE("Test completed board") {
    constexpr SudokuBoard b{"126389457738425169549617832374852916892176543651943728983561274417238695265794381"};
    REQUIRE(b.isFull());
    REQUIRE(b.isDone());
}

TEST_CASE("Test error counting") {
    // Errors in this board: 6
    // 1 error:  two 9s in row 0
    // 3 errors: three 1s in col 0
    // 1 error:  two 1s in grid (0,0)
    // 1 error:  two 9s in grid (1,2)
    constexpr SudokuBoard b{
            "102987459"
            "134652798"
            "785134260"
            "100000000"
            "000000000"
            "000000000"
            "000000000"
            "000000000"
            "000000000"
    };
    REQUIRE(!b.isFull());
    REQUIRE(b.findNumberOfErrors() == 6);
}

// Convert a set of positions as pairs to a set of positions as indices.
std::set<size_t> pairToPos(const std::set<std::pair<size_t, size_t>> &pairs) {
    std::set<size_t> pos{};
    std::transform(std::cbegin(pairs), std::cend(pairs),
            std::inserter(pos, std::begin(pos)),
            [] (const auto &p) {
        return p.first * 9 + p.second;
    });
    return std::move(pos);
}

TEST_CASE("Test findEmptyPositions") {
    constexpr SudokuBoard b{
        "102345678"
        "098765430"
        "444000444"
        "987654321"
        "001234500"
        "635281263"
        "090807060"
        "654321987"
        "000000000"
    };

    std::set<std::pair<size_t, size_t>> pairs{
            {0, 1},
            {1, 0}, {1, 8},
            {2, 3}, {2, 4}, {2, 5},
            {4, 0}, {4, 1}, {4, 7}, {4, 8},
            {6, 0}, {6, 2}, {6, 4}, {6, 6}, {6, 8},
            {8, 0}, {8, 1}, {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 6}, {8, 7}, {8, 8}
    };
    REQUIRE(pairToPos(pairs) == b.findEmptyPositions());
}

TEST_CASE("Test hasValidEntries") {
    // A base-17 board with valid entries.
    constexpr GenSudokuBoard<4> b {
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