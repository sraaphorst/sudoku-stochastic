#include <iostream>

#include <GenSudokuBoard.h>
#include <GenSudokuBoardPopulator.h>
#include <GeneticAlgorithm.h>

#include "Timer.h"

using namespace vorpal::gensudoku;
using namespace vorpal::stochastic;

int main() {
    run_timed("sudoku", []() {
        SudokuBoard board(
                "100089457738000000040010000004050906000000000000000728080001000007008095060090300");

        using solver = GeneticAlgorithm<SudokuBoard, size_t>;
        solver::Options options;
        //options.selector = std::make_unique<RandomSelector<SudokuBoard>>();
        //options.selector = std::make_unique<RouletteSelector<SudokuBoard>>();
        options.selector = std::make_unique<KTournamentSelector<SudokuBoard>>(2);
        options.crossover_probability = 0.7;
        options.fitness_success_threshold = SudokuBoard::PerfectFitness;
        options.fitness_death_threshold = static_cast<size_t>(SudokuBoard::PerfectFitness * 0.95);
        options.max_generations = 100;
        options.mutation_probability = 0.9;
        options.populator = std::make_unique<SudokuBoardPopulator>(board);
        options.population_size = 10000;

        const auto &sol = solver::run(options);
        std::cout << "Best solution found has fitness " << sol->fitness() << ":\n";
        for (size_t row = 0; row < 9; ++row) {
            for (size_t col = 0; col < 9; ++col)
                std::cout << (*sol)[row * 9 + col];
            std::cout << '\n';
        }
    });
}