/**
 * GenSudokuBoardPopulator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <omp.h>

#include <algorithm>
#include <stdexcept>
#include <vector>

#include <boost/format.hpp>

#include "Populator.h"
#include "GenSudokuBoard.h"

namespace vorpal::gensudoku {
    /**
     * Abstract superclass to generate GenSudokuBoards.
     * Contains the base board.
     *
     * @tparam N the size parameter
     */
    template<size_t N = 3, const auto NN = N * N, const auto BoardSize = NN * NN>
    class GenSudokuBoardPopulator: public stochastic::Populator<GenSudokuBoard<N>> {
    protected:
        // The partial Sudoku board, with 0s representing unfilled cells.
        GenSudokuBoard<N> partial_board;

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

        ~GenSudokuBoardPopulator() = default;

    private:
        void initialize() {
            // Create a set to copy from.
            std::vector<size_t> base_set{NN};
            base_set.resize(NN);
            std::iota(std::begin(base_set), std::end(base_set), 1);

            // Determine what numbers can go in what space.
            #pragma omp parallel for
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

                const auto gridrow = row / NN;
                const auto gridcol = col / NN;

                // Remove the row, column and grid entries.
                for (size_t i = 0; i < NN; ++i) {
                    const auto row_entry = partial_board[i * NN + col];
                    const auto col_entry = partial_board[row * NN + i];

                    // Get the grid entry.
                    const auto grididx =(gridrow * N + i / N) * NN + (gridcol * N + i % N);
                    const  auto grid_entry = partial_board[grididx];

                    if (row_entry != 0) vec.erase(std::remove(vec.begin(), vec.end(), row_entry), vec.end());
                    if (col_entry != 0) vec.erase(std::remove(vec.begin(), vec.end(), col_entry), vec.end());
                    if (grid_entry != 0) vec.erase(std::remove(vec.begin(), vec.end(), grid_entry), vec.end());
                }
                if (vec.empty())
                    throw std::invalid_argument(boost::format("invalid board: position (%1%,%2%) has no candidate").str());
                cell_candidates[idx] = vec;
            }
        }
    };
}
