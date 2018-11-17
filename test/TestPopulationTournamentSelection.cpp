///**
// * TestPopulationTournamentSelection.cpp
// *
// * By Sebastian Raaphorst, 2018.
// */
//
//#include <catch.hpp>
//
//#include <array>
//#include <functional>
//#include <iostream>
//
//#include "PopulationSelector.h"
//#include "GenSudokuBoard.h"
//#include "GenSudokuBoardPopulator.h"
//
//using namespace vorpal::gensudoku;
//using namespace vorpal::stochastic;
//
//TEST_CASE("Choose a population of size k") {
//    SudokuBoard b{"100089457"
//                  "738000000"
//                  "040010000"
//                  "004050906"
//                  "000000000"
//                  "000000728"
//                  "080001000"
//                  "007008095"
//                  "060090300"};
//    REQUIRE(!b.isFull());
//
//    SudokuBoardPopulator filler{b};
//    std::vector<std::unique_ptr<SudokuBoard>> sudokus{50};
//    for (int i = 0; i < 50; ++i)
//        sudokus[i] = filler.generate();
//
//    for (int i = 0; i < 10; ++i) {
//        std::cerr << "Picking... ";
//        const auto candidate = KTournamentSelection::selection(5, std::begin(sudokus), std::end(sudokus));
//        std::cerr << "Candidate " << i << " has fitness " << (*candidate)->fitness() << '\n';
//    }
//}
