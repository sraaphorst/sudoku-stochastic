/**
 * sudoku_sa.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */


#include <iostream>
#include <memory>
#include <stdexcept>

#include <boost/program_options.hpp>

#include <GenSudokuBoard.h>
#include <GenSudokuBoardAscenderPopulator.h>
#include <SimulatedAnnealingAlgorithm.h>

#include "progopts.h"

namespace po = boost::program_options;
using namespace vorpal::gensudoku;
using namespace vorpal::stochastic;

int main(int argc, const char * const argv[]) {
    using solver = SimulatedAnnealingAlgorithm<SudokuBoard>;
    solver::options_type options;

    try {
        auto desc = init_options(options);
        desc->add_options()
                ("initial-temperature,t", po::value<double>()->default_value(10000), "initial temperature")
                ("alpha,a", po::value<double>()->default_value(0.99), "alpha (cooling rate)");
        auto done = end_options<SudokuBoardAscenderPopulator, solver::options_type>(argc, argv, desc, options,
        [](po::variables_map &vm, solver::options_type &options) {
            options.initial_temperature = vm["initial-temperature"].as<double>();
            options.alpha               = vm["alpha"].as<double>();
        });
        if (done) return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    run<solver>(options);
}
