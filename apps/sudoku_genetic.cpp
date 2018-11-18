#include <iostream>

#include <GenSudokuBoard.h>
#include <GenSudokuBoardPopulator.h>
#include <GeneticAlgorithm.h>

#include "Timer.h"

using namespace vorpal::gensudoku;
using namespace vorpal::stochastic;

int main() {
    run_timed("sudoku", []() {
        SudokuBoard easy_board("870090230000003010004281000040809603090060040308704090000527100010900000027040085");
        SudokuBoard impossible_board("800000000003600000070090200050007000000045700000100030001000068008500010090000400");

        // Configure the solver.
        using solver = GeneticAlgorithm<SudokuBoard, size_t>;
        solver::Options options;
        options.populator = std::make_unique<SudokuBoardPopulator>(easy_board);//(impossible_board);
        options.selector = std::make_unique<KTournamentSelector<SudokuBoard>>(3);
        options.fitness_success_threshold = SudokuBoard::PerfectFitness;
        // Having this set to 0 made a huge difference in one trial.: 1904 - 1935 - 1936 - 1938
        //options.fitness_death_threshold = static_cast<size_t>(SudokuBoard::PerfectFitness * 0.98);
        options.fitness_death_factor = 0.99;//0.9875;
        options.permissible_dead_rounds = 500'000;

        const auto &sol = solver::run(options);
        std::cout << "Best solution found has fitness " << sol->fitness() << ":\n";
        for (size_t row = 0; row < 9; ++row) {
            for (size_t col = 0; col < 9; ++col)
                std::cout << (*sol)[row * 9 + col];
            std::cout << '\n';
        }
    });
}