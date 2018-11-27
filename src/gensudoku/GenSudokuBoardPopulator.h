/**
 * GenSudokuBoardPopulator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

#include <boost/format.hpp>

#include "Populator.h"
#include "RNG.h"
#include "GenSudokuBoard.h"
#include "Populator.h"
#include "RNG.h"

#include <iostream>

namespace vorpal::gensudoku {
    /**
     * Abstract superclass to generate GenSudokuBoards.
     * Contains the base board.
     *
     * @tparam N the size parameter
     */
    template<size_t N = 3,
            const auto NN = N * N,
            const auto BoardSize = NN * NN>
    class GenSudokuBoardPopulator: public stochastic::Populator<GenSudokuBoard<N>> {
    protected:
        // The partial Sudoku board, with 0s representing unfilled cells.
        GenSudokuBoard<N> partial_board;

        std::vector<std::vector<size_t>> rowEmptyPositions;
        std::vector<std::vector<size_t>> rowMissingEntries;

        // Refine the possible contents of the cells, indexed by position, in a sorted collection.
        // This will allow O(1) access when randomly picking an element.
        std::vector<std::vector<size_t>> cell_candidates;

    public:
        using pointer_type = std::unique_ptr<GenSudokuBoard<N>>;

        GenSudokuBoardPopulator() = delete;
        GenSudokuBoardPopulator(const GenSudokuBoardPopulator&) = default;
        GenSudokuBoardPopulator(GenSudokuBoardPopulator&&) = default;
        GenSudokuBoardPopulator &operator=(const GenSudokuBoardPopulator&) = default;

        explicit GenSudokuBoardPopulator(const GenSudokuBoard<N> &partial_board):
            partial_board{partial_board}, cell_candidates{BoardSize} {
            initialize();
        }

        explicit GenSudokuBoardPopulator(GenSudokuBoard<N> &&partial_board):
            partial_board{std::move(partial_board)}, cell_candidates{BoardSize} {
            initialize();
        }

        virtual ~GenSudokuBoardPopulator() = default;

        /**
         * Generate a random board from the partial board this class was initialized it.
         * @return a random board
         */
        virtual pointer_type generate() noexcept override {
            // For each row, shuffle the missing entries and distribute them amongst the empty positions.
            auto board = std::make_unique<GenSudokuBoard<N>>(this->partial_board);

            for (size_t row = 0; row < NN; ++row)
                fillRow(board, row);
            return board;
        }

    protected:
        /**
         * Given a board and a row, permute the missing entries and use them to fill the row.
         * This is common code to generating boards and mutating a board.
         * We use the cell_contents that we precomputed to make sure the rows are viable.
         * @param board the board
         * @param row the row to fill
         */
        void fillRow(std::unique_ptr<GenSudokuBoard<N>> &board, size_t row) noexcept {
            // Repeatedly shuffle the candidates for the row and try to insert until we can.
            auto &gen = stochastic::RNG::getGenerator();
            for (;;) {
                std::vector<size_t> &entries = rowMissingEntries[row];
                std::shuffle(std::begin(entries), std::end(entries), gen);

                bool flag = true;
                for (size_t col = 0; col < entries.size(); ++col) {
                    const auto pos = row * NN + rowEmptyPositions[row][col];
                    const auto cend = std::cend(cell_candidates[pos]);
                    if (std::find(std::cbegin(cell_candidates[pos]), cend, entries[col]) == cend) {
                        flag = false;
                    break;
                }
                (*board)[pos] = entries[col];
            }
            if (flag)
                break;
        }
    }

    protected:
        void initialize() {
            // Create a set to copy from.
            std::vector<size_t> base_set{NN};
            base_set.resize(NN);
            std::iota(std::begin(base_set), std::end(base_set), 1);

            // Determine what numbers can go in what space.
            for (size_t idx = 0; idx < BoardSize; ++idx) {
                // If the cell is fixed by default, it only has one candidate.
                if (partial_board[idx] != 0) {
                    cell_candidates[idx] = {partial_board[idx]};
                    continue;
                }

                // Make a copy of base set and modify it subtractively by basic refinement.
                auto vec = base_set;
                const auto row = idx / NN;
                const auto col = idx % NN;

                const auto gridrow = row / N;
                const auto gridcol = col / N;

                // Remove the row, column and grid entries.
                for (size_t i = 0; i < NN; ++i) {
                    const auto row_entry = partial_board[i * NN + col];
                    const auto col_entry = partial_board[row * NN + i];

                    // Get the grid entry.
                    const auto grididx =(gridrow * N + i / N) * NN + (gridcol * N + i % N);
                    const auto grid_entry = partial_board[grididx];
                    if (row_entry != 0) vec.erase(std::remove(vec.begin(), vec.end(), row_entry), vec.end());
                    if (col_entry != 0) vec.erase(std::remove(vec.begin(), vec.end(), col_entry), vec.end());
                    if (grid_entry != 0) vec.erase(std::remove(vec.begin(), vec.end(), grid_entry), vec.end());
                }
                if (vec.empty())
                    throw std::invalid_argument((boost::format("invalid board: position (%1%,%2%) has no candidate") % row % col).str());
                cell_candidates[idx] = vec;
            }

            // We iterate over each row, finding the missing positions and empty entries.
            for (size_t row = 0; row < NN; ++row) {
                std::bitset<NN> emptyPositions;
                emptyPositions.reset();
                std::bitset<NN + 1> presentEntries;
                presentEntries.reset();

                for (size_t col = 0; col < NN; ++col) {
                    const size_t pos = row * NN + col;
                    if (this->partial_board[pos] == 0)
                        emptyPositions[col] = true;
                    else
                        presentEntries[this->partial_board[pos]] = true;
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
