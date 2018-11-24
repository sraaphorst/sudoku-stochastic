/**
 * GenSudokuBoardBHCPopulator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "DefaultMethods.h"
#include "GenSudokuBoardHCPopulator.h"
#include "RNG.h"

namespace vorpal::gensudoku {
    template<size_t N = 3,
            const auto NN = N * N,
            const auto BoardSize = NN * NN>
    class GenSudokUBoardBHCPopulator:
            public GenSudokUBoardHCPopulator {

        // The probability in beta-mutating a cell that can be mutated.
        const double prob_b;

    public:
        constexpr static double DEFAULT_B_PROBABILITY = 0.5;

        __GENSUDOKUBOARD_POPULATOR_GENERIC_CONSTRUCTORS(GenSudokUBoardBHCPopulator)

        explicit GenSudokUBoardBHCPopulator(const data_type &partial_board,
                double prob_n = DEFAULT_N_PROBABILITY, double prob_b = DEFAULT_B_PROBABILITY):
                GenSudokuBoardHCPopulator{partial_board, prob_n}, prob_b{prob_b} {}

        explicit GenSudokUBoardBHCPopulator(data_type &&partial_board,
                                            double prob_n = DEFAULT_N_PROBABILITY, double prob_b = DEFAULT_B_PROBABILITY):
                GenSudokuBoardHCPopulator{std::forward<data_type&&>(partial_board), prob_n}, prob_b{prob_b} {}

    private:
        pointer_type bOperator(const pointer_type &current_board) const {
            auto &gen = stochastic::RNG::getGenerator();

        }
    };
}