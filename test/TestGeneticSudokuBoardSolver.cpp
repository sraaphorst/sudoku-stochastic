/**
 * TestGeneticSudokuBoardSolver.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <memory>

#include "GenSudokuBoard.h"
#include "GenSudokuBoardPopulator.h"

#include "GeneticAlgorithm.h"
#include "PopulationSelector.h"

using namespace vorpal::gensudoku;
using namespace vorpal::stochastic;

TEST_CASE("Solve a simple sudoku puzzle") {
    constexpr SudokuBoard board("100089457738000000040010000004050906000000000000000728080001000007008095060090300");

    using solver = GeneticAlgorithm<SudokuBoard, size_t>;
    solver::Options options;
    //options.selector = std::make_unique<RandomSelector<SudokuBoard>>();
    //options.selector = std::make_unique<RouletteSelector<SudokuBoard>>();
    options.selector = std::make_unique<KTournamentSelector<SudokuBoard>>(5);
    options.crossover_probability = 0.8;
    options.fitness_success_threshold = SudokuBoard::PerfectFitness;
    options.max_generations = 100;
    options.mutation_probability = 0.75;
    options.populator = std::make_unique<SudokuBoardPopulator>(board);
    options.population_size = 500;

    const auto &sol = solver::run(options);
    std::cerr << "Best solution found has fitness " << sol->fitness() << ":\n";
    for (size_t row = 0; row < 9; ++row) {
        for (size_t col = 0; col < 9; ++col)
            std::cerr << (*sol)[row * 9 + col];
        std::cerr << '\n';
    }
}