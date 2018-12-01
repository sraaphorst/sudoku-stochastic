/**
 * sudoku_tl.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */


#include <iostream>
#include <memory>
#include <stdexcept>

#include <boost/program_options.hpp>

#include <GenSudokuBoard.h>
#include <GenSudokuBoardAscenderPopulator.h>
#include <TabuSearchAlgorithm.h>

#include "progopts.h"

namespace po = boost::program_options;
using namespace vorpal::gensudoku;
using namespace vorpal::stochastic;

int main(int argc, const char * const argv[]) {
    using solver = TabuSearchAlgorithm<SudokuBoard>;
    solver::options_type options;

    try {
        auto desc = init_options(options);
        desc->add_options()
                ("tabu-list-length,t", po::value<size_t>()->default_value(20), "tabu list length");
        auto done = end_options<SudokuBoardAscenderPopulator, solver::options_type>(argc, argv, desc, options,
        [](po::variables_map &vm, solver::options_type &options) {
            options.tabu_list_length = vm["tabu-list-length"].as<size_t>();
        });
        if (done) return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    run<solver>(options);
}
