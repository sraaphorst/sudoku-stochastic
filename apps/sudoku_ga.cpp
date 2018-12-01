/**
 * sudoku_ga.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include <GenSudokuBoard.h>
#include <GenSudokuBoardGAPopulator.h>
#include <GeneticAlgorithm.h>
#include <PredefinedBoards.h>

#include "timer.h"

namespace po = boost::program_options;
using namespace vorpal::gensudoku;
using namespace vorpal::stochastic;


void conflicting_options(const po::variables_map &vm, const char * const opt1, const char * const opt2) {
    if (vm.count(opt1) && !vm[opt1].defaulted() && vm.count(opt2) && !vm[opt2].defaulted())
        throw std::logic_error((boost::format("Incompatible options: %1% amd %2%") % opt1 % opt2).str());
}

double must_be_01(const po::variables_map &vm, const char * const opt) {
    double p = vm[opt].as<double>();
    if (p < 0 || p > 1)
        throw std::invalid_argument((boost::format("Invalid value for %1%: %2%") % opt % p).str());
    return p;
}

int main(int argc, const char *argv[]) {
    using solver = GeneticAlgorithm<SudokuBoard, size_t>;
    solver::options_type options;

    try {
        po::options_description desc{"options_type"};
        desc.add_options()
                ("help,h", "Display help information")
                ("population-size,P", po::value<size_t>()->default_value(options.population_size), "Size of population")
                ("max-generations,M", po::value<uint64_t>()->default_value(options.max_generations), "Maximum number of generations")
                ("ktournament,k", po::value<size_t>()->default_value(3), "Use a k-tournament selector (default) with k participants")
                ("random,r", "Use a random selector")
                ("roulette,R", "Use a roulette selector")
                ("crossover-probability,c", po::value<double>()->default_value(options.crossover_probability),
                        "Probability of a crossover, i.e. that breeding will occur to replace an older population member")
                ("mutation-probability,m", po::value<double>()->default_value(options.mutation_probability),
                        "Probability of mutation in a population member")
                ("fitness-death-factor,f", po::value<double>()->default_value(options.fitness_death_factor),
                        "Population members that are not within this amount of the best member will be killed")
                ("permissible-dead-rounds,p", po::value<uint64_t>()->default_value(options.permissible_dead_rounds),
                        "Number of rounds permitted to see no improvement before the whole population is killed")
                ("suppress_logging,s", "Suppress logging")
                ("output-rounds,o", po::value<uint64_t>()->default_value(options.output_rounds),
                        "Log output after this many rounds");

        po::options_description hidden{"Hidden options"};
        hidden.add_options()
                ("board", po::value<std::vector<std::string>>(), "board");
        po::positional_options_description p;
        p.add("board", 1);

        po::options_description pall;
        pall.add(desc).add(hidden);

        po::variables_map vm;
        store(po::command_line_parser(argc, argv).
                options(pall).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << '\n';
            std::cout << "Note that board may also be one of: very_easy_board, easy_board, medium_board, very_hard_board, "
                         "impossible_board, or benchmark_board, with default benchmark_board\n";
            return 0;
        }

        // Try to get the board.
        std::vector<std::string> board_vec = vm.count("board") ? vm["board"].as<std::vector<std::string>>() :
                std::vector<std::string>{};
        std::string board_str = board_vec.empty() ? "benchmark_board" : board_vec[0];

        std::optional<SudokuBoard> board = PredefinedBoards::lookupBoard(board_str);
        options.populator = std::make_unique<SudokuBoardGAPopulator>(board ?
                *board : SudokuBoard(board_str));

        conflicting_options(vm, "ktournament", "random");
        conflicting_options(vm, "ktournament", "roulette");
        conflicting_options(vm, "random", "roulette");

        options.population_size = vm["population-size"].as<size_t>();
        options.max_generations = vm["max-generations"].as<uint64_t>();
        options.permissible_dead_rounds = vm["permissible-dead-rounds"].as<uint64_t>();

        if (vm.count("random"))
            options.selector = std::make_unique<RandomSelector<SudokuBoard>>();
        else if (vm.count("roulette"))
            options.selector = std::make_unique<RouletteSelector<SudokuBoard>>();
        else
            options.selector = std::make_unique<KTournamentSelector<SudokuBoard>>(vm["ktournament"].as<size_t>());

        options.crossover_probability = must_be_01(vm, "crossover-probability");
        options.mutation_probability  = must_be_01(vm, "mutation-probability");
        options.fitness_death_factor  = must_be_01(vm, "fitness-death-factor");

        // We can only have output-rounds if logging is on.
        options.output_rounds = vm.count("suppress-logging") ? UINT64_MAX : vm["output-rounds"].as<uint64_t>();

        options.fitness_success_threshold = 243;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    run_timed("sudoku", [&options]() {
        const auto &sol = solver::run(options);
        std::flush(std::clog);
        std::cout << "Best solution found has fitness " << sol->fitness() << ":\n";
        for (size_t row = 0; row < 9; ++row) {
            for (size_t col = 0; col < 9; ++col)
                std::cout << (*sol)[row * 9 + col];
            std::cout << '\n';
        }
        std::flush(std::cout);
    });
}
