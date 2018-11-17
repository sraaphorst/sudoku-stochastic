/**
 * GenSudokuBoard.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <omp.h>

#include <array>
#include <iterator>
#include <map>
#include <mutex>
#include <stdexcept>
#include <string_view>
#include <unordered_set>

#include <boost/format.hpp>

#include "Candidate.h"
#include "ConstMath.h"

namespace vorpal::gensudoku {
    template<size_t, size_t> class GenSudokuBoardPopulator;

    /**
     * A generic sudoku board of size parameter N, i.e. the board has:
     * 1. N^2 rows;
     * 2. N^2 columns;
     * 3. N x N subgrids of size N x N.
     * 3. N^2 symbols [1,N^2], and a reserved symbol, 0, that indicates the board cell is not yet fixed.
     *
     * Note that we don't worry about here, as we want to use OpenMP, which is not compatible.
     *
     * @tparam N the size parameter
     */
    template<size_t N = 3,
            const auto NN = N * N,
            const auto BoardSize = NN * NN>
    class GenSudokuBoard final: public stochastic::Candidate<GenSudokuBoard<N>> {
    public:
        /**
         * The contents of a board. We represent as a single flat array of length N^4 to simplify certain
         * algorithms and checks.
         */
        using board_contents = std::array<size_t, BoardSize>;

    private:
        board_contents contents{};

    public:
        constexpr GenSudokuBoard() noexcept = default;
        constexpr GenSudokuBoard(const GenSudokuBoard&) noexcept = default;
        constexpr GenSudokuBoard(GenSudokuBoard&&) noexcept = default;

        constexpr explicit GenSudokuBoard(const std::string_view &sv) {
            if (sv.size() != BoardSize)
                throw std::invalid_argument("Sudoku board must be initialized with flat string of appropriate length");

            for (int i = 0; i < BoardSize; ++i)
                contents[i] = fromBase36(sv[i]);
        }

        constexpr explicit GenSudokuBoard(const board_contents &c) {
            checkContents(c);
            contents = c;
        }

        constexpr explicit GenSudokuBoard(board_contents &&c) {
            checkContents(c);
            contents = std::move(c);
        }

        constexpr GenSudokuBoard &operator=(const GenSudokuBoard&) noexcept = default;

        constexpr bool operator==(const GenSudokuBoard &other) noexcept {
            return contents == other.contents;
        };
        constexpr bool operator!=(const GenSudokuBoard &other) noexcept {
            return !(*this == other);
        };

        /**
         * Accessors.
         */
        constexpr const size_t operator[](const std::pair<size_t, size_t> &pos) const noexcept {
            const auto [row, col] = pos;
            return contents[row * NN + col];
        }

        constexpr size_t &operator[](const std::pair<size_t, size_t> &pos) noexcept {
            const auto [row, col] = pos;
            assert(row < NN && col < NN);
            return contents[row * NN + col];
        }

        constexpr const size_t operator[](size_t pos) const noexcept {
            assert(pos < BoardSize);
            return contents[pos];
        }

        constexpr size_t &operator[](size_t pos) noexcept {
            assert(pos < BoardSize);
            return contents[pos];
        }

        /**
         * Determine if the board is full, i.e. it contains no zeroes.
         * @return true if full, false otherwise
         */
        constexpr bool isFull() const noexcept {
            return std::find(std::cbegin(contents), std::cend(contents), 0) == std::cend(contents);
        }

        /**
         * Determine if a board has valid entries, i.e. entries in the required range.
         * We also allow zero.
         * @return true if the board has valid entries, and false otherwise.
         */
        bool hasValidEntries() const noexcept {
             bool flag = true;

             #pragma omp parallel for shared(flag)
             for (size_t i = 0; i < BoardSize; ++i) {
                 if (!flag) continue;
                 if (contents[i] > NN)
                     flag = false;
             }

             return flag;
         }

        /**
         * Determine if a board is done. This will only be the case when the board is complete and has no errors.
         */
        constexpr bool isDone() const noexcept {
            return isFull() && hasValidEntries() && findNumberOfErrors() == 0;
        }


        /**
         * Calculate the number of errors on a Sudoku board, i.e. the number of violations that occur.
         * @return the number of errors
         */
        size_t findNumberOfErrors() const noexcept {
            size_t errorCount = 0;

            #pragma omp parallel for shared(errorCount, contents)
            for (size_t pos = 0; pos < BoardSize; ++pos) {
                size_t errors = findNumberOfErrors(pos);
                if (errors > 0) {
                    #pragma omp atomic update
                    errorCount += errors;
                }
            }

            return errorCount;
        }

        /**
         * Return the fitness of the board, i.e. the maxinum number of possible errors minus the number of errors.
         * @return maxerrors - errors
         */
        size_t fitness() const override {
            return 3 * BoardSize * (NN - 1) - findNumberOfErrors();
        }

    private:
        /**
         * A static method that, given a contents, determines if the members of the contents are legal, i.e.
         * either 0 (the special empty marker), or [1, N^2].
         * @tparam B to make universal reference to contents
         * @param contents the contents to check
         */
        template<typename B>
        static void checkContents(B &&contents) {
            for (size_t i = 0; i < BoardSize; ++i)
                if (contents[i] > NN)
//                    throw std::invalid_argument(boost::format("illegal digit at position (%1%,%2$): %3%")
//                                                % (i / 9) % (i % 9) % contents[i]);
                    throw std::invalid_argument("no");
        }

        /**
         * Static convenience method to take a position and return the row it is in.
         * @param pos position
         * @return the row
         */
        constexpr static size_t posToRow(size_t pos) noexcept {
            assert(pos < BoardSize);
            return pos / NN;
        }

        /**
         * Static convenience method to take a position and return the column it is in.
         * @param pos position
         * @return the column
         */
        constexpr static size_t posToCol(size_t pos) {
            assert(pos < BoardSize);
            return pos % NN;
        }

        /**
         * Static convenience method to take a position and return what grid it is in.
         * @param pos position
         * @return (i,j) representing the grid
         */
        constexpr static std::pair<size_t, size_t> posToGrid(size_t pos) {
            assert(pos < BoardSize);
            const size_t gridx = (pos / NN) / N;
            const size_t gridy = (pos % NN) / N;
            assert(gridx < N);
            assert(gridy < N);
            return {gridx, gridy};
        }

        /**
         * This method determines, given the current layout, the number of errors in the board.
         * An error is defined by a cell whose contents violate either a row restriction, a column restriction,
         * or a grid restriction.
         *
         * For example, having two 1s in a row would be considered a row restriction violation.
         * For positions where the contents are not yet set, there is no restriction violation.
         *
         * @param pos the position for which to calculate restrictions
         * @return the number of restrictions that the contents of this position induces
         */
        size_t findNumberOfErrors(size_t pos) const {
            assert(pos < BoardSize);

            // If this cell is empty, it can't have errors.
            if (contents[pos] == 0)
                return 0;

            size_t rowerrors  = 0;
            size_t colerrors  = 0;
            size_t griderrors = 0;

            const auto digit = contents[pos];
            const auto row   = posToRow(pos);
            const auto col   = posToCol(pos);

            // pos falls into the grid (xgrid, ygrid).
            const auto gridXY = posToGrid(pos);

            // We can't bind gridXY and then pass it in.
            #pragma omp parallel for shared(rowerrors, colerrors, griderrors, contents, row, col, digit, gridXY)
            for (size_t i = 0; i < NN; ++i) {
                // Handle row errors.
                const size_t rowpos = row * NN + i;
                if (rowpos < pos && contents[rowpos] == digit) {
                    #pragma omp atomic update
                    ++rowerrors;
                }

                // Handle column errors.
                const size_t colpos = i * NN + col;
                if (colpos < pos && contents[colpos] == digit) {
                    #pragma omp atomic update
                    ++rowerrors;
                }

                // Handle grid errors.
                const auto [gridX, gridY] = gridXY;
                const size_t gridrow = i / N;
                const size_t gridcol = i % N;
                const size_t gridpos = NN * (N * gridX + gridrow) + N * gridY + gridcol;
                if (gridpos < pos && contents[gridpos] == digit) {
                    #pragma omp atomic update
                    ++rowerrors;
                }
            }

            // Now we return the error count - 3 because we counted the digit in this position three times.
            assert(rowerrors  < NN);
            assert(colerrors  < NN);
            assert(griderrors < NN);
            return rowerrors + colerrors + griderrors;
        }

        friend class GenSudokuBoardPopulator<N, NN>;
    };

    using SudokuBoard = GenSudokuBoard<>;
}
