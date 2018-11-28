/**
 * sudoku_tl.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>
#include <memory>

#include <GenSudokuBoard.h>
#include <GenSudokuBoardHCPopulator.h>
#include <PredefinedBoards.h>
#include <TabuSearchAlgorithm.h>

#include "Timer.h"

using namespace vorpal::gensudoku;
using namespace vorpal::stochastic;

int main() {
    run_timed("sudoku", []() {
        // Configure the solver.
        using solver = TabuSearchAlgorithm<SudokuBoard>;
        solver::option_type options;
        options.populator = std::make_unique<SudokuBoardHCPopulator>(PredefinedBoards::very_easy_board);
        options.fitness_success_threshold = SudokuBoard::PerfectFitness;
        options.tabu_list_length = 20;

        const auto &sol = solver{}.run(options);
        std::cerr << "Best solution found has fitness " << sol->fitness() << ":\n";
        for (size_t row = 0; row < 9; ++row) {
            for (size_t col = 0; col < 9; ++col)
                std::cerr << (*sol)[row * 9 + col];
            std::cerr << '\n';
        }
    });
}