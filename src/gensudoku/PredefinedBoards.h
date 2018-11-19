/**
 * PredefinedBoards.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

namespace vorpal::gensudoku {

    /**
     * A collection of predefined Sudoku boards for use in the heuristic algorithms.
     */
    struct PredefinedBoards {
        static constexpr SudokuBoard very_easy_board{"040053102208100700501420600814030207060205019050740063000074581185902000403008026"};
        static constexpr SudokuBoard easy_board1{"870090230000003010004281000040809603090060040308704090000527100010900000027040085"};
        static constexpr SudokuBoard easy_board2{"800070024603804000007009068400080600050960000020007180069700000704500200000006017"};
        static constexpr SudokuBoard impossible_board{"800000000003600000070090200050007000000045700000100030001000068008500010090000400"};
    };
}