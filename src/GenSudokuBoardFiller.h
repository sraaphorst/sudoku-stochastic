/**
 * GenSudokuFiller.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <omp.h>

#include <algorithm>
#include <bitset>
#include <iterator>
#include <random>
#include <unordered_set>
#include <vector>

#include "GenSudokuBoard.h"

namespace sudoku_stochastic {
    /**
     * Class of static methods to fill in a partially completed Sudoku board.
     * @tparam N the size parameter
     */
    template<size_t N=3,
            const auto NN = N * N>
    class GenSudokuBoardFiller {
        GenSudokuBoard<N> partial_board;
        std::vector<std::vector<size_t>> rowEmptyPositions;
        std::vector<std::vector<size_t>> rowMissingEntries;

    public:
        // A collection of static methods.
        GenSudokuBoardFiller() = delete;
        GenSudokuBoardFiller(const GenSudokuBoardFiller&) = default;
        GenSudokuBoardFiller(GenSudokuBoardFiller&&) = default;
        GenSudokuBoardFiller &operator=(const GenSudokuBoardFiller&) = delete;

        explicit GenSudokuBoardFiller(const GenSudokuBoard<N> &partial_board):
        partial_board(partial_board), rd{}, g{rd()} {
            initialize();
        }
        explicit GenSudokuBoardFiller(GenSudokuBoard<N> &&partial_board):
        partial_board(std::move(partial_board)), rd{}, g{rd()} {
            initialize();
        }

        GenSudokuBoard<N> generateRandomBoard() noexcept {
            // For each row, shuffle the missing entries and distribute them amongst the empty positions.
            GenSudokuBoard<N> board = partial_board;

            #pragma omp parallel for shared(board)
            for (size_t row = 0; row < NN; ++row) {
                std::vector<size_t> entries = rowMissingEntries[row];
                std::shuffle(std::begin(entries), std::end(entries), g);
                for (size_t col = 0; col < entries.size(); ++col) {
                    size_t pos = row * NN + rowEmptyPositions[row][col];
                    size_t val = entries[col];
                    board[pos] = entries[col];
                }
            }
            return board;
        }

    private:
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

        std::random_device rd;
        std::mt19937 g;
    };
}