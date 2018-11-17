/**
 * GenSudokuFiller.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <algorithm>
#include <bitset>
#include <random>
#include <tuple>
#include <vector>

#include "Candidate.h"
#include "Populator.h"
#include "GenSudokuBoard.h"
#include "RNG.h"

namespace vorpal::gensudoku {
    /**
     * Implementation of Populator to produce new GenSudokuBoards.
     * @tparam N the size parameter
     */
    template<size_t N=3,
            const size_t NN = N * N>
    class GenSudokuBoardPopulator final: public stochastic::Populator<GenSudokuBoard<N>> {
        GenSudokuBoard<N> partial_board;
        std::vector<std::vector<size_t>> rowEmptyPositions;
        std::vector<std::vector<size_t>> rowMissingEntries;

    public:
        using pointer_type = std::unique_ptr<GenSudokuBoard<N>>;

        // A collection of static methods.
        GenSudokuBoardPopulator() = delete;
        GenSudokuBoardPopulator(const GenSudokuBoardPopulator&) = default;
        GenSudokuBoardPopulator(GenSudokuBoardPopulator&&) = default;
        GenSudokuBoardPopulator &operator=(const GenSudokuBoardPopulator&) = default;

        explicit GenSudokuBoardPopulator(const GenSudokuBoard<N> &partial_board):partial_board{partial_board} {
            initialize();
        }

        explicit GenSudokuBoardPopulator(GenSudokuBoard<N> &&partial_board): partial_board{std::move(partial_board)} {
            initialize();
        }

        ~GenSudokuBoardPopulator() = default;

        /**
         * Generate a random board from the partial board this class was initialized it.
         * @return a random board
         */
        pointer_type generate() noexcept override {
            // For each row, shuffle the missing entries and distribute them amongst the empty positions.
            auto board = std::make_unique<GenSudokuBoard<N>>(partial_board);

            for (size_t row = 0; row < NN; ++row)
                fillRow(board, row);
            return board;
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
            fillRow(board, row);

            return board;
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

            for (int i = 0; i < NN; ++i) {
                if (distribution(gen) == 0) {
                    for (int j = 0; j < NN; ++j) {
                        (*c0)[i * NN + j] = p0->contents[i * NN + j];
                        (*c1)[i * NN + j] = p1->contents[i * NN + j];
                    }
                } else {
                    for (int j = 0; j < NN; ++j) {
                        (*c0)[i * NN + j] = p1->contents[i * NN + j];
                        (*c1)[i * NN + j] = p0->contents[i * NN + j];
                    }
                }
            }

            return {std::move(c0), std::move(c1)};
        }

        pointer_type survive(const pointer_type &p) noexcept override {
            pointer_type c = std::make_unique<GenSudokuBoard<N>>(*p);
            return std::move(c);
        }

    private:
        /**
         * Given a board and a row, permute the missing entries and use them to fill the row.
         * This is common code to generating boards and mutating a board.
         * @param board the board
         * @param row the row to fill
         */
        void fillRow(std::unique_ptr<GenSudokuBoard<N>> &board, size_t row) noexcept {
            // Shuffle the candidates for the row.
            auto &gen = stochastic::RNG::getGenerator();

            std::vector<size_t> entries = rowMissingEntries[row];
            std::shuffle(std::begin(entries), std::end(entries), gen);

            for (size_t col = 0; col < entries.size(); ++col) {
                size_t pos = row * NN + rowEmptyPositions[row][col];
                (*board)[pos] = entries[col];
            }
        }

        /**
         * Iterate over each row of the partial board, and determine what entries are missing and from what positions.
         * We only perform this once, so there is no point to parallelization.
         */
        void initialize() noexcept {
            // We iterate over each row, finding the missing positions and empty entries.
            for (size_t row = 0; row < NN; ++row) {
                std::bitset<NN> emptyPositions;
                emptyPositions.reset();
                std::bitset<NN + 1> presentEntries;
                presentEntries.reset();

                for (size_t col = 0; col < NN; ++col) {
                    const size_t pos = row * NN + col;
                    if (partial_board[pos] == 0)
                        emptyPositions[col] = true;
                    else
                        presentEntries[partial_board[pos]] = true;
                }

                // Conglomerate the data.
                std::vector<size_t> rEmptyPositions;
                std::vector<size_t> rMissingEntries;
                for (size_t col = 0; col < NN; ++col) {
                    if (emptyPositions[col] == 1)
                        rEmptyPositions.emplace_back(col);
                    if (presentEntries[col + 1] == 0)
                        rMissingEntries.emplace_back(col + 1);
                }
                assert(rEmptyPositions.size() == rMissingEntries.size());
                rowEmptyPositions.emplace_back(std::move(rEmptyPositions));
                rowMissingEntries.emplace_back(std::move(rMissingEntries));
            }
        }
    };

    using SudokuBoardPopulator = GenSudokuBoardPopulator<>;
}