/**
 * sudoku_gd.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>
#include <memory>
#include <stdexcept>

#include <boost/program_options.hpp>

#include <GenSudokuBoard.h>
#include <GenSudokuBoardAscenderPopulator.h>
#include <GreatDelugeAlgorithm.h>

#include "progopts.h"

namespace po = boost::program_options;
using namespace vorpal::gensudoku;
using namespace vorpal::stochastic;

int main(int argc, const char * const argv[]) {
    using solver = GreatDelugeAlgorithm<SudokuBoard>;
    solver::options_type options;

    try {
        auto desc = init_options(options);
        desc->add_options()
                ("initial-water-level,w", po::value<double>()->default_value(100.0), "initial water level")
                ("rain-speed,r", po::value<double>()->default_value(0.1), "rain speed");
        auto done = end_options<SudokuBoardAscenderPopulator, solver::options_type>(argc, argv, desc, options,
        [](po::variables_map &vm, solver::options_type &options) {
            options.initial_water_level = vm["initial-water-level"].as<double>();
            options.rain_speed          = vm["rain-speed"].as<double>();
        });
        if (done) return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    run<solver>(options);
}
