/**
 * TestPopulationTournamentSelection.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <array>
#include <iostream>

#include "PopulationSelector.h"
#include "GenSudokuBoard.h"
#include "GenSudokuBoardPopulator.h"

using namespace vorpal::gensudoku;
using namespace vorpal::stochastic;

TEST_CASE("Choose a population of size k") {
    constexpr size_t NumBoards = 50;

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

    SudokuBoardPopulator filler{b};
    std::vector<std::unique_ptr<SudokuBoard>> sudokus;
    for (size_t i = 0; i < NumBoards; ++i)
        sudokus.emplace_back(filler.generate());

    const KTournamentSelector<SudokuBoard> selector{5};
    for (int i = 0; i < 10; ++i) {
        std::cerr << "Picking... ";
        const auto candidate = selector.select(sudokus);
        REQUIRE(candidate < NumBoards);
        REQUIRE(sudokus[candidate]->fitness() <= SudokuBoard::PerfectFitness);

        std::cerr << "Candidate " << i << " has fitness " << sudokus[candidate]->fitness() << '\n';
    }
}
