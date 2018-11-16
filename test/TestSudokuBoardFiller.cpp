/**
 * TestSudokuBoardFiller.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <bitset>

#include <SudokuBoard.h>
#include <SudokuBoardFiller.h>

using namespace sudoku_stochastic;

TEST_CASE("Create a board where every row is a permutation") {
    SudokuBoard b{"100089457"
                  "738000000"
                  "040010000"
                  "004050906"
                  "000000000"
                  "000000728"
                  "080001000"
                  "007008095"
                  "060090300"};
    REQUIRE(!b.isFull());
    SudokuBoardFiller filler{b};
    for (int bnum = 0; bnum < 100; ++bnum) {
        const auto board = filler.generateRandomBoard();

        // Check each row.
        for (size_t row = 0; row < 9; ++row) {
            std::bitset<9> positions{};
            positions.reset(0);
            std::bitset<10> entries{};
            entries.reset(0);

            for (size_t col = 0; col < 9; ++col) {
                size_t pos = 9 * row + col;
                if (board[pos] != 0) {
                    positions[col] = true;
                    size_t entry = board[pos];
                    entries[entry] = true;
                }
            }
            REQUIRE(positions.count() == 9);
            REQUIRE(entries.count() == 9);
        }
    }
}