/**
 * sudoku_hc.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>
#include <memory>

#include <GenSudokuBoard.h>
#include <GenSudokuBoardHCPopulator.h>
#include <AscenderAlgorithm.h>
#include <PredefinedBoards.h>

#include "Timer.h"

using namespace vorpal::gensudoku;
using namespace vorpal::stochastic;

int main() {
    run_timed("sudoku", []() {
        // Configure the solver.
        using solver = AscenderAlgorithm<SudokuBoard, size_t>;
        solver::Options options;
        options.populator = std::make_unique<SudokuBoardHCPopulator>(PredefinedBoards::impossible_board);
        options.fitness_success_threshold = SudokuBoard::PerfectFitness;

        const auto &sol = solver::run(options);
        std::cout << "Best solution found has fitness " << sol->fitness() << ":\n";
        for (size_t row = 0; row < 9; ++row) {
            for (size_t col = 0; col < 9; ++col)
                std::cout << (*sol)[row * 9 + col];
            std::cout << '\n';
        }
    });
}