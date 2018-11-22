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
        options.populator = std::make_unique<SudokuBoardGAPopulator>(PredefinedBoards::impossible_board);
        options.selector = std::make_unique<KTournamentSelector<SudokuBoard>>(4);
        options.fitness_success_threshold = SudokuBoard::PerfectFitness;
        options.fitness_death_factor = 0.8;
        options.permissible_dead_rounds = 50'000;
        options.crossover_probability = 0.5;
        options.mutation_probability = 0.1;
        options.population_size = 500;

        const auto &sol = solver::run(options);
        std::cout << "Best solution found has fitness " << sol->fitness() << ":\n";
        for (size_t row = 0; row < 9; ++row) {
            for (size_t col = 0; col < 9; ++col)
                std::cout << (*sol)[row * 9 + col];
            std::cout << '\n';
        }
    });
}