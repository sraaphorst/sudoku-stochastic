/**
 * GenSudokuFiller.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <omp.h>

#include <algorithm>
#include <bitset>
#include <random>
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
            const auto NN = N * N>
    class GenSudokuBoardPopulator final: public stochastic::Populator {
        GenSudokuBoard<N> partial_board;
        std::vector<std::vector<size_t>> rowEmptyPositions;
        std::vector<std::vector<size_t>> rowMissingEntries;

    public:
        // A collection of static methods.
        GenSudokuBoardPopulator() = delete;
        GenSudokuBoardPopulator(const GenSudokuBoardPopulator&) noexcept = default;
        GenSudokuBoardPopulator(GenSudokuBoardPopulator&&) noexcept = default;
        GenSudokuBoardPopulator &operator=(const GenSudokuBoardPopulator&) = delete;

        explicit GenSudokuBoardPopulator(const GenSudokuBoard<N> &partial_board):partial_board{partial_board} {
            initialize();
        }

        explicit GenSudokuBoardPopulator(GenSudokuBoard<N> &&partial_board): partial_board{std::move(partial_board)} {
            initialize();
        }

        /**
         * Generate a random board from the partial board this class was initialized it.
         * @return a random board
         */
        std::unique_ptr<stochastic::Candidate> generate() noexcept override {
            // For each row, shuffle the missing entries and distribute them amongst the empty positions.
            auto board = std::make_unique<GenSudokuBoard<N>>(partial_board);

            #pragma omp parallel for shared(board)
            for (size_t row = 0; row < NN; ++row)
                fillRow(board, row);
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
        std::unique_ptr<stochastic::Candidate> mutate(const GenSudokuBoard<N> &original_board) noexcept override {
            auto board = std::make_unique<GenSudokuBoard<N>>(original_board);

            // Select a row at random and shuffle the candidates.
            auto gen = stochastic::RNG::getGenerator();
            const size_t row = std::uniform_int_distribution<size_t>{0, NN - 1}(gen);
            fillRow(board, row);

            return std::move(board);
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
            auto gen = stochastic::RNG::getGenerator();

            std::vector<size_t> entries = rowMissingEntries[row];
            std::shuffle(std::begin(entries), std::end(entries), gen);

            #pragma omp parallel for shared(board, entries, gen)
            for (size_t col = 0; col < entries.size(); ++col) {
                size_t pos = row * NN + rowEmptyPositions[row][col];
                size_t val = entries[col];

                //#pragma omp atomic write
                board[pos] = entries[col];
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