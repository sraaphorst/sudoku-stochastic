/**
 * TestPopulationTournamentSelection.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <array>
#include <functional>
#include <iostream>

#include "genetic/PopulationSelector.h"
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

    std::cerr << "1\n";
    SudokuBoardFiller filler{b};
    std::cerr << "2\n";
    std::vector<SudokuBoard> sudokus{50};
    for (int i = 0; i < 50; ++i) {
        std::cerr << "\t" << i << '\n';
        sudokus[i] = filler.generateRandomBoard();
        std::cerr << "\t" << i << '\n';
    }
    std::cerr << "3\n";
    for (int i = 00; i < 10; ++i) {
        std::cerr << "Picking...\n";
        const auto candidate = genetic::tournamentSelection(5, std::begin(sudokus), std::end(sudokus),
                                                             [](const auto &s) { return s.fitness(); });
        std::cerr << "Candidate " << i << " has fitness " << candidate->fitness() << '\n';
    }
}