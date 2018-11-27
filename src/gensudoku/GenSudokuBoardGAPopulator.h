/**
 * GenSudokuBoardGAPopulator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once
#include <algorithm>
#include <bitset>
#include <memory>
#include <random>
#include <tuple>

#include "DefaultMethods.h"
#include "GeneticPopulator.h"
#include "GenSudokuBoard.h"
#include "GenSudokuBoardPopulator.h"
#include "RNG.h"

namespace vorpal::gensudoku {
    /**
     * Implementation of GeneticPopulator to produce new GenSudokuBoards.
     * @tparam N the size parameter
     */
    template<size_t N=3,
            const auto NN = N * N>
    class GenSudokuBoardGAPopulator final:
            public virtual GenSudokuBoardPopulator<N>,
            public virtual stochastic::GeneticPopulator<GenSudokuBoard<N>> {

    public:
        __GENSUDOKUBOARD_POPULATOR_CONSTRUCTORS(GenSudokuBoardGAPopulator, GenSudokuBoardPopulator);

        /**
         * Generate a random board from the partial board this class was initialized it.
         * @return a random board
         */
        pointer_type generate() noexcept override {
            // For each row, shuffle the missing entries and distribute them amongst the empty positions.
            auto board = std::make_unique<GenSudokuBoard<N>>(this->partial_board);
            for (size_t row = 0; row < NN; ++row)
                this->fillRow(board, row);
            return std::move(board);
        }

        /**
         * Given a board, perform a mutation operation on it.
         * The mutation operation takes a single row index and randomly distributes the missing entries over the row.
         *
         * The reason to include mutations in this class is because the row information is readily available, which is
         * necessary to perform the mutation.
         *
         * @param originalBoard the original board
         * @return the board with the row mutated
         */
        pointer_type mutate(const pointer_type &original_board)
                    noexcept override {
            auto board = std::make_unique<GenSudokuBoard<N>>(*original_board);

            // Select a row at random and shuffle the candidates.
            auto &gen = stochastic::RNG::getGenerator();
            const size_t row = std::uniform_int_distribution<size_t>{0, NN - 1}(gen);
            this->fillRow(board, row);

            return std::move(board);
        }

        /**
         * Breed two parent solutions using uniform crossover to produce two child solutions.
         * @param p0 first parent
         * @param p1 second parent
         * @return two children from the crossover breeding.
         */
        std::pair<pointer_type, pointer_type> crossover(const pointer_type &p0, const pointer_type &p1) noexcept override {
            // For each of the children, for each row, we do a "crossover" where one child takes the row of one parent
            // and the other child the row of the other parent.
            auto c0 = std::make_unique<GenSudokuBoard<N>>();
            auto c1 = std::make_unique<GenSudokuBoard<N>>();

            // Iterate over the rows and assign them to the children.
            auto &gen = stochastic::RNG::getGenerator();
            std::uniform_int_distribution distribution{0, 1};

            for (size_t i = 0; i < NN; ++i) {
                if (distribution(gen) == 0) {
                    for (size_t j = 0; j < NN; ++j) {
                        (*c0)[i * NN + j] = p0->contents[i * NN + j];
                        (*c1)[i * NN + j] = p1->contents[i * NN + j];
                    }
                } else {
                    for (size_t j = 0; j < NN; ++j) {
                        (*c0)[i * NN + j] = p1->contents[i * NN + j];
                        (*c1)[i * NN + j] = p0->contents[i * NN + j];
                    }
                }
            }

            return {std::move(c0), std::move(c1)};
        }

        pointer_type survive(const pointer_type &p) noexcept override {
            pointer_type c = std::make_unique<data_type>(*p);
            return std::move(c);
        }
    };

    using SudokuBoardGAPopulator = GenSudokuBoardGAPopulator<>;
}