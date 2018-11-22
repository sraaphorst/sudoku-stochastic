/**
 * GenSudokuBoardHCPopulator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "AscenderPopulator.h"
#include "GenSudokuBoardPopulator.h"
#include "RNG.h"

namespace vorpal::gensudoku {
    /**
     * The implementation for the most basic form of hill climbing.
     * @tparam N the size parameter
     */
    template<size_t N, const auto NN = N * N>
    class GenSudokuBoardHCPopulator:
            public GenSudokuBoardPopulator<N>,
            public stochastic::AscenderPopulator<GenSudokuBoard<N>> {
    public:
        using pointer_type = std::unique_ptr<GenSudokuBoard<N>>;

        GenSudokuBoardGAPopulator() = delete;
        GenSudokuBoardGAPopulator(const GenSudokuBoardGAPopulator&) = default;
        GenSudokuBoardGAPopulator(GenSudokuBoardGAPopulator&&) = default;
        GenSudokuBoardGAPopulator &operator=(const GenSudokuBoardGAPopulator&) = default;

        explicit GenSudokuBoardGAPopulator(const GenSudokuBoard<N> &partial_board):
                GenSudokuBoardPopulator<N>{partial_board} {
            initialize();
        }

        explicit GenSudokuBoardGAPopulator(GenSudokuBoard<N> &&partial_board):
        GenSudokuBoardPopulator<N>{partial_board} {
            initialize();
        }

        ~GenSudokuBoardGAPopulator() = default;
    };
}
/**
 * A modification of a hill climbing algorithm as discovered by:
 *
 * Mohammed Azmi Al-Betar. 2017. $\beta$-Hill climbing: an exploratory local search.
 * Neural Computing and Applications 28, 1 (January 2017), pp. 153-168.
 *
 * M. A. Al-Betar, M. A. Awadallah, A. L. Bolaji and B. O. Alijla, $\beta$-Hill Climbing Algorithm for Sudoku Game.
 * 2017 Palestinian International Conference on Information and Communication Technology (PICICT), Gaza, Palestine, 2017, pp. 84-88.
 *
 * Note that by omitting the beta-function, a regular hill climber is achieved.
 */