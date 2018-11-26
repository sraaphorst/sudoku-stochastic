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
    /**
     * A modification of a hill climbing algorithm as discovered by:
     *
     * Mohammed Azmi Al-Betar. 2017. $\beta$-Hill climbing: an exploratory local search.
     * Neural Computing and Applications 28, 1 (January 2017), pp. 153-168.
     *
     * M. A. Al-Betar, M. A. Awadallah, A. L. Bolaji and B. O. Alijla, $\beta$-Hill Climbing Algorithm for Sudoku Game.
     * 2017 Palestinian International Conference on Information and Communication Technology (PICICT), Gaza, Palestine, 2017, pp. 84-88.
     *
     * Note that by omitting the beta-function, a regular hill climber is achieved.
     */
    template<size_t N = 3,
            const auto NN = N * N,
            const auto BoardSize = NN * NN>
    class GenSudokUBoardBHCPopulator:
            public GenSudokuBoardHCPopulator<N> {

        // The probability in beta-mutating a cell that can be mutated.
        const double prob_b;

    public:
        constexpr static double DEFAULT_B_PROBABILITY = 0.5;

        __GENSUDOKUBOARD_POPULATOR_GENERIC_CONSTRUCTORS(GenSudokUBoardBHCPopulator)

        explicit GenSudokUBoardBHCPopulator(const data_type &partial_board,
                double prob_n = GenSudokuBoardHCPopulator<N>::DEFAULT_N_PROBABILITY,
                double prob_b = DEFAULT_B_PROBABILITY):
                GenSudokuBoardHCPopulator<N>{partial_board, prob_n}, prob_b{prob_b} {}

        explicit GenSudokUBoardBHCPopulator(data_type &&partial_board,
                double prob_n = GenSudokuBoardHCPopulator<N>::DEFAULT_N_PROBABILITY,
                double prob_b = DEFAULT_B_PROBABILITY):
                GenSudokuBoardHCPopulator<N>{std::forward<data_type&&>(partial_board), prob_n}, prob_b{prob_b} {}

        virtual pointer_type selectedNeighbour(int iteration, pointer_type &current) const override {
            auto nneighbour = std::move(this->nOperator(current));
            auto bneighbour = std::move(bOperator(nneighbour));
            //std::cerr << "o=" << current->fitness() << ", n=" << nneighbour->fitness() << ", b=" << bneighbour->fitness() << '\n';

            if (bneighbour->fitness() > current->fitness())
                return std::move(bneighbour);
            else
                return std::move(current);
        }

    private:
        pointer_type bOperator(const pointer_type &current_board) const {
            auto &gen = stochastic::RNG::getGenerator();
            std::uniform_real_distribution<> distribution;

            // Mutate current by the B-operator, which does a probability check for every non-fixed cell.
            // If the probability check succeeds, it changes the value of the cell to a random candidate.
            pointer_type neighbour_board = std::make_unique<data_type>(*current_board);

            for (size_t pos = 0; pos < BoardSize; ++pos) {
                const auto &candidates = this->cell_candidates[pos];
                if (candidates.size() > 1 && distribution(gen) < prob_b) {
                    std::uniform_int_distribution<size_t> idistribution(0, candidates.size() - 1);
                    (*neighbour_board)[pos] = candidates[idistribution(gen)];
                }
            }

            return neighbour_board;
        }
    };

    using SudokuBoardBHCPopulator = GenSudokUBoardBHCPopulator<>;
}