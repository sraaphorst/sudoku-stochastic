/**
 * TestPopulationTournamentSelection.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <array>
#include <functional>

#include "PopulationTournamentSelection.h"
#include "SudokuBoard.h"
#include "SudokuBoardFiller.h"

using namespace sudoku_stochastic;

TEST_CASE("Choose a population of size k") {
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

    std::vector<SudokuBoard> sudokus{50};
    for (int i = 0; i < 50; ++i)
        sudokus[i] = filler.generateRandomBoard();

    // Pick 10 from sets of 5.
    std::vector<SudokuBoard> out{5};

    selections::tournamentSelection(5, std::begin(sudokus), std::end(sudokus), std::begin(out),
            [](const auto &s1, const auto &s2) { return s1.fitness() < s2.fitness(); });
    std::vector<size_t> candidates{10};
    //for (int i = 0; i < 10; ++i)

}