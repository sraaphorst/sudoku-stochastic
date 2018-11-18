/**
 * GenSudokuBoard.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
#include <bitset>
#include <iterator>
#include <iostream>
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
     * Note that fitness for Sudoku puzzles is discrete, so we use size_t instead of double.
     * @tparam N the size parameter
     */
    template<size_t N = 3,
            const auto NN = N * N,
            const auto BoardSize = NN * NN>
    class GenSudokuBoard final: public stochastic::Candidate<GenSudokuBoard<N>, size_t> {
    public:
        /**
         * The perfect fitness of the board is indicated by every digit appearing in every row, column, and grid.
         */
        constexpr static auto PerfectFitness = 3 * NN * NN;
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
            return isFull() && hasValidEntries() && fitness() == PerfectFitness;
        }

        /**
         * Return the fitness of the board, i.e. the number of unique digits in each row, column, and grid.
         * Note that rows
         * @return maxerrors - errors
         */
        size_t fitness() const override {
            // Row fitness is always 81, as  every row is a permutation.
            const size_t row_fit = 81;

            // Iterate over columns.
            size_t col_fit = 0;
            for (size_t col = 0; col < NN; ++col) {
                std::bitset<NN+1> col_set;

                for (size_t row = 0; row < NN; ++row)
                    col_set[contents[row * NN + col]] = true;
                col_set[0] = false;
                col_fit += col_set.count();
            }

            // Iterate over the subgrids.
            size_t grid_fit = 0;
            for (size_t gridx = 0; gridx < N; ++gridx) {
                for (size_t gridy = 0; gridy < N; ++gridy) {
                    std::bitset<NN+1> grid_set;

                    for (size_t gridrow = 0; gridrow < N; ++gridrow)
                        for (size_t gridcol = 0; gridcol < N; ++gridcol)
                            grid_set[contents[(gridx * N + gridrow) * NN + gridy * N + gridcol]] = true;
                    grid_set[0] = 0;
                    grid_fit += grid_set.count();
                }
            }

            return row_fit + col_fit + grid_fit;
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
                    throw std::invalid_argument(boost::format("illegal digit at position (%1%,%2$): %3%")
                                                % (i / 9) % (i % 9) % contents[i]);
        }

        friend class GenSudokuBoardPopulator<N, NN>;
    };

    using SudokuBoard = GenSudokuBoard<>;
}
