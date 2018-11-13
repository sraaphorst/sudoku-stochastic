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
#include <set>
#include <stdexcept>
#include <string_view>
#include <boost/format.hpp>
#include <iostream>

#include "constmath.h"

namespace sudoku_stochastic {
    /**
     * A generic sudoku board of size parameter N, i.e. the board has:
     * 1. N^2 rows;
     * 2. N^2 columns;
     * 3. N x N subgrids of size N x N.
     * 3. N^2 symbols [1,N^2], and a reserved symbol, 0, that indicates the board cell is not yet fixed.
     * @tparam N the size parameter
     */
    template<size_t N = 3,
            const auto NN = N * N,
            const auto BoardSize = NN * NN>
    class GenSudokuBoard final {
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

        explicit constexpr GenSudokuBoard(const std::string_view &sv) {
            if (sv.size() != BoardSize)
                throw std::invalid_argument("Sudoku board must be initialized with flat string of appropriate length");

            for (int i = 0; i < BoardSize; ++i)
                contents[i] = math::fromBase36(sv[i]);
        }

        explicit constexpr GenSudokuBoard(const board_contents &c) {
            check_contents(c);
            contents = c;
        }

        explicit GenSudokuBoard(board_contents &&c) {
            check_contents(c);
            contents = std::move(c);
        }

        GenSudokuBoard &operator=(const GenSudokuBoard&) noexcept = default;

        bool operator==(const GenSudokuBoard &other) noexcept {
            return contents == other.contents;
        };
        bool operator!=(const GenSudokuBoard &other) noexcept {
            return !(*this == other);
        };

        /**
         * Accessors.
         */
        const size_t operator[](const std::pair<size_t, size_t> &pos) const noexcept {
            const auto [row, col] = pos;
            return contents[row * NN + col];
        }

        size_t &operator[](const std::pair<size_t, size_t> &pos) noexcept {
            const auto [row, col] = pos;
            assert(row < NN && col < NN);
            return contents[row * NN + col];
        }

        const size_t operator[](size_t pos) const noexcept {
            assert(pos < BoardSize);
            return contents[pos];
        }

        size_t &operator[](size_t pos) noexcept {
            assert(pos < BoardSize);
            return contents[pos];
        }


        /**
         * Determine if the board is complete, i.e. it contains no zeroes.
         * @return true if complete, false otherwise
         */
        bool isComplete() const noexcept {
            return std::find(std::cbegin(contents), std::cend(contents), 0) == std::cend(contents);
        }

        /**
         * Find the set of empty positions in the board.
         * @return the empty positions
         */
        std::set<size_t> findEmptyPositions() const noexcept {
            std::mutex set_add;
            std::set<size_t> empty;

            #pragma omp parallel for shared(empty, contents)
            for (size_t i = 0; i < BoardSize; ++i) {
                if (contents[i] == 0) {
                    std::lock_guard<std::mutex> guard{set_add};
                    empty.insert(i);
                }
            }

            return empty;
        }

        /**
         * Determine if a board is done. This will only be the case when the board is complete and has no errors.
         */
        bool isDone() const noexcept {
            return isComplete() && findErrorRepresentative() == 0;
        }

        /**
         * Return a map of the count of the digits in the board.
         * This can be used, for example, to randomly populate the rest of the board while making
         * sure that each element appears the same number of times.
         * @return
         */
        std::map<size_t, size_t> findDigitCounts() const noexcept {
            std::mutex digit_add;

            // Note that a map of this type will return 0 for undefined keys, which is exactly what we want.
            std::map<size_t, size_t> digitCounts;
            #pragma omp parallel for shared(digitCounts, contents)
            for (size_t pos = 0; pos < BoardSize; ++pos) {
                std::lock_guard<std::mutex> guard{digit_add};
                ++digitCounts[contents[pos]];
            }
            return digitCounts;
        }

        size_t findErrorRepresentative() const noexcept {
            std::mutex error_add;
            size_t errorCount = 0;

            //#pragma omp parallel for shared(errorCount, contents)
            for (size_t pos = 0; pos < BoardSize; ++pos) {
                size_t errors = findNumberOfErrors(pos);
                if (errors > 0) {
                    std::lock_guard<std::mutex> guard{error_add};
                    errorCount += errors;
                }
            }

            return errorCount;
        }

    private:
        /**
         * A static method that, given a contents, determines if the members of the contents are legal, i.e.
         * either 0 (the special empty marker), or [1, N^2].
         * @tparam B to make universal reference to contents
         * @param contents the contents to check
         */
        template<typename B>
        static void check_contents(B &&contents) {
            for (size_t i = 0; i < BoardSize; ++i)
                if (contents[i] > NN)
                    throw std::invalid_argument(boost::format("illegal digit at position (%1%,%2$): %3%")
                    % (i / 9) % (i % 9) % contents[i]);
        }

        /**
         * Static convenience method to take a position and return the row it is in.
         * @param pos position
         * @return the row
         */
        static size_t posToRow(size_t pos) noexcept {
            assert(pos < BoardSize);
            return pos / NN;
        }

        /**
         * Static convenience method to take a position and return the column it is in.
         * @param pos position
         * @return the column
         */
        static size_t posToCol(size_t pos) {
            assert(pos < BoardSize);
            return pos % NN;
        }

        /**
         * Static convenience method to take a position and return what grid it is in.
         * @param pos position
         * @return (i,j) representing the grid
         */
        static std::pair<size_t, size_t> posToGrid(size_t pos) {
            assert(pos < BoardSize);
            const size_t gridx = (pos / NN) % N;
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
         * Note that the sum of this method across all cells does not provide the exact number of
         * restriction violations in the board, as there is overcounting: for example, if 1 appears twice in row 0,
         * it will count as two row restrictions. If it appears thrice, it will count at 3! = 6 restrictions.
         *
         * @param pos the position for which to calculate restrictions
         * @return the number of restrictions that the contents of this position induces
         */
        size_t findNumberOfErrors(size_t pos) const {
            assert(pos < BoardSize);

            // If this cell is empty, it can't have errors.
            if (contents[pos] == 0)
                return 0;

            std::mutex row_mutex, col_mutex, grid_mutex;
            size_t rowerrors = 0;
            size_t colerrors = 0;
            size_t griderrors = 0;

            size_t row = pos / NN;
            size_t col = pos % NN;

            // 00 01 02 00 01 02
            // 10 11 12 10 11 12
            //size_t gridx = pos / N;
            //size_t gridy = pos % NN;
            size_t gridx = pos / NN / N;
            size_t gridy = (pos % NN) / N;

            size_t digit = contents[pos];

//            //#pragma omp parallel for shared(rowerrors, colerrors, griderrors, contents)
//            for (size_t i = 0; i < NN; ++i) {
//                // Check row.
//                if (contents[row * NN + i] == dig) {
//                    std::lock_guard<std::mutex> guard{row_mutex};
//                    ++rowerrors;
//                    std::cerr<< "Error in row pos=" << pos << ", pos2=" << (row * NN + i) << ", value=" << dig << '\n';
//                }
//                // Check column.
//                if (contents[i * NN + col] == dig) {
//                    std::lock_guard<std::mutex> guard{col_mutex};
//                    ++colerrors;
//                    std::cerr<< "Error in col pos=" << pos << ", pos2=" << (i * NN + col) << ", value=" << dig << '\n';
//                }
//
//                // Check grid.
//                const size_t xrow = xgrid * N + i / N;
//                const size_t yrow = ygrid * N + i % N;
//                //const size_t gpos = (xgrid * N + xgridpos) * N + (ygrid * N + ygridpos);
//                const size_t gpos = xrow * NN + yrow;
//                std::cerr << "pos=" << pos << ", (" << xgrid << "," << ygrid << ") = gridpos=" << gpos << '\n';
//                if (contents[gpos] == dig) {
//                    std::lock_guard<std::mutex> guard{grid_mutex};
//                    ++griderrors;
//                    std::cerr<< "Error in grid pos=" << pos << ", gpos=" << gpos << ", value=" << dig << '\n';
//                }
//            }
            std::cerr << "***** POSITION " << row << "," << col << " *****\n";
            std::cerr << "*** ROWS ***\n";
            std::cerr << "Looking in row " << row << ":";
            for (size_t i = 0; i < NN; ++i) {
                std::cerr << ' ' << (row * NN + i);
                if (contents[row * NN + i] == digit) {
                    std::lock_guard<std::mutex> guard{row_mutex};
                    ++rowerrors;
                    std::cerr << ':' << i;
                }
            }
            std::cerr << "\n";

            std::cerr << "*** COLS ***\n";
            std::cerr << "Looking in column " << row << ":";
            for (size_t i = 0; i < NN; ++i) {
                std::cerr << ' ' << (i * NN + col);
                if (contents[i * NN + col] == digit) {
                    std::lock_guard<std::mutex> guard{row_mutex};
                    ++rowerrors;
                    std::cerr << ':' << i;
                }
            }
            std::cerr << '\n';

            std::cerr << "*** GRIDS ***\n";
            std::cerr << "Looking in grid (" << gridx << ',' << gridy << "):";
            for (size_t i = 0; i < NN; ++i) {
                const size_t grow = i / N;
                const size_t gcol = i % N;
                //const size_t gpos = (N * gridx + grow) * NN + (N * gridy + gcol) * N;
                const size_t gpos = NN * (N * gridx + grow) + N * gridy + gcol;
                std::cerr << " ("  << grow << "," << gcol << ':' << (N * gridx + grow) << "," << (N * gridy + gcol) << ":" << gpos << ")";
                if (contents[gpos] == digit) {
                    std::lock_guard<std::mutex> guard{row_mutex};
                    ++rowerrors;
                    std::cerr << ':' << i;
                }
            }
            std::cerr << "\n";

            // Now we return the error count - 3 because we counted the digit in this position three times.
            const size_t errors = rowerrors + colerrors + griderrors - 3;
            std::cerr << "ERRORS: " << rowerrors << " + " << colerrors << " + " << griderrors << " -3 = " << errors << "\n\n";
            assert(errors <= BoardSize - 3);
            return errors;
        }
    };
}
