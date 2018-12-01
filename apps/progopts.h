/**
 * progopts.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include <boost/program_options.hpp>

#include <PredefinedBoards.h>
#include <GenSudokuBoard.h>
#include <HillClimbingAlgorithm.h>

#include "timer.h"

namespace po = boost::program_options;

using HCOptions = vorpal::stochastic::HillClimbingOptions<vorpal::gensudoku::SudokuBoard>;

std::unique_ptr<po::options_description> init_options(const HCOptions &options) {
    auto desc = std::make_unique<po::options_description>("options_type");
    desc->add_options()
            ("help,h", "Display help information")
            ("max-iterations-per-round,m", po::value<uint64_t>()->default_value(options.max_iterations_per_round),
                  "Maximum number of iterations per round before restarting")
            ("max-rounds", po::value<uint64_t>()->default_value(options.max_rounds),
                  "Maximum number of rounds");
    return desc;
}

template<typename Populator, typename OptionsType>
bool end_options(int argc, const char * const argv[],
        std::unique_ptr<po::options_description> &desc,
        OptionsType &options,
        std::function<void(po::variables_map&, OptionsType &)> configurator = [](auto&, auto&){}) {
    static_assert(std::is_base_of_v<HCOptions, OptionsType>);

    po::options_description hidden("Hidden options");
    hidden.add_options()
            ("board", po::value<std::vector<std::string>>(), "board");
    po::positional_options_description p;
    p.add("board", 1);

    po::options_description pall;
    pall.add(*desc).add(hidden);

    po::variables_map vm;
    store(po::command_line_parser(argc, argv).
        options(pall).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << '\n';
        std::cout << "Note that board may also be one of: very_easy_board, easy_board, medium_board, very_hard_board, "
                     "impossible_board, or benchmark_board, with default benchmark_board\n";
        return true;
    }

    // Try to get the board.
    std::vector<std::string> board_vec = vm.count("board") ? vm["board"].as<std::vector<std::string>>() :
                                         std::vector<std::string>{};
    std::string board_str = board_vec.empty() ? "benchmark_board" : board_vec[0];

    std::optional<vorpal::gensudoku::SudokuBoard> board = vorpal::gensudoku::PredefinedBoards::lookupBoard(board_str);
    options.populator = std::make_unique<Populator>(board ? *board : vorpal::gensudoku::SudokuBoard(board_str));

    options.max_iterations_per_round = vm["max-iterations-per-round"].as<uint64_t>();
    options.max_rounds = vm["max-rounds"].as<uint64_t>();
    options.fitness_success_threshold = vorpal::gensudoku::SudokuBoard::PerfectFitness;

    configurator(vm, options);
    return false;
}

template<typename Solver>
void run(const typename Solver::options_type &options) {
    run_timed("sudoku", [&options]() {
        // Configure the solver.
        const auto sol = Solver{}.run(options);
        std::clog << std::flush;
        std::cout << std::flush;
        std::cerr << std::flush;
        std::cout << "Best solution found has fitness " << sol->fitness() << ":\n";
        for (size_t row = 0; row < 9; ++row) {
            for (size_t col = 0; col < 9; ++col)
                std::cout << (*sol)[row * 9 + col];
            std::cout << '\n';
        }
        std::cout << std::flush;

        std::clog << std::flush;
        std::cout << std::flush;
        std::cerr << std::flush;
    });
}
