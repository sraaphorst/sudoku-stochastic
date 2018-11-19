#include <iostream>

#include <GenSudokuBoard.h>
#include <GenSudokuBoardGAPopulator.h>
#include <GeneticAlgorithm.h>
#include <PredefinedBoards.h>

#include "Timer.h"

using namespace vorpal::gensudoku;
using namespace vorpal::stochastic;

int main() {
    run_timed("sudoku", []() {
        // Configure the solver.
        using solver = GeneticAlgorithm<SudokuBoard, size_t>;
        solver::Options options;
        options.populator = std::make_unique<SudokuBoardGAPopulator>(PredefinedBoards::easy_board1);
        options.selector = std::make_unique<KTournamentSelector<SudokuBoard>>(2);
        options.fitness_success_threshold = SudokuBoard::PerfectFitness;
        // Having this set to 0 made a huge difference in one trial.: 1904 - 1935 - 1936 - 1938
        //options.fitness_death_threshold = static_cast<size_t>(SudokuBoard::PerfectFitness * 0.98);
        options.fitness_death_factor = 0.8;
        options.permissible_dead_rounds = 1'000'000;
        options.crossover_probability = 0.5;
        options.mutation_probability = 0.4;

        const auto &sol = solver::run(options);
        std::cout << "Best solution found has fitness " << sol->fitness() << ":\n";
        for (size_t row = 0; row < 9; ++row) {
            for (size_t col = 0; col < 9; ++col)
                std::cout << (*sol)[row * 9 + col];
            std::cout << '\n';
        }
    });
}