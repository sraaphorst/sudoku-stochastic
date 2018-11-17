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
    options.selector = std::make_unique<RandomSelector<SudokuBoard>>();
    options.crossover_probability = 0.8;
    options.fitness_success_threshold = SudokuBoard::PerfectFitness;
    options.max_generations = 10;
    options.mutation_probability = 0.75;
    options.populator = std::make_unique<SudokuBoardPopulator>(board);
    options.population_size = 50;
    options.min_populator_size = 3;

    solver::run(options);
}