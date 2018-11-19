/**
 * GenSudokuHillClimberPopulator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "AscenderPopulator.h"
#include "GenSudokuBoard.h"
#include "GenSudokuBoardPopulator.h"
#include "RNG.h"

namespace vorpal::gensudoku {
    template<size_t N = 3, const auto NN = N * N>
    class GenSudokuHCPopulator: public AscenderPopulator, public GenSudokuBoardPopulator<N> {

    public:
        using pointer_type = std::unique_ptr<GenSudokuBoard<N>>;

        GenSudokuHCPopulator() = delete;
        GenSudokuHCPopulator(const GenSudokuHCPopulator&) = default;
        GenSudokuHCPopulator(GenSudokuHCPopulator&&) = default;

        explicit GenSudokuHCPopulator(const GenSudokuBoard<N> &partial_board):
            GenSudokuBoardPopulator<N>{partial_board} {}
        explicit GenSudokuHCPopulator(GenSudokuBoard<N> &&partial_board):
                GenSudokuBoardPopulator<N>{partial_board} {}


        virtual ~GenSudokuHCPopulator() = default;

        pointer_type generate() noexcept override {
            auto board = std::make_unique<GenSudokuBoard<N>>(partial_board);

            // Fill the empty slots on the board with any valid number.
            auto &gen = stochastic::RNG::getGenerator();

            for (size_t pos = 0; pos < NN; ++pos) {
                const auto value = std::uniform_int_distribution<size_t>(0, cell_candidates.size() - 1)(gen);
                board[pos] = value;
            }

            return board;
        }
    };
}