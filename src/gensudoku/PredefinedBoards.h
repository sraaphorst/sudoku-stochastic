/**
 * PredefinedBoards.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <optional>

namespace vorpal::gensudoku {

    /**
     * A collection of predefined Sudoku boards for use in the heuristic algorithms.
     */
    struct PredefinedBoards {
        static constexpr SudokuBoard very_easy_board {"040053102208100700501420600814030207060205019050740063000074581185902000403008026"};
        static constexpr SudokuBoard easy_board      {"870090230000003010004281000040809603090060040308704090000527100010900000027040085"};
        static constexpr SudokuBoard medium_board    {"009056010067410000300700020070930800050000030004085060090003002000029340020860900"};
        static constexpr SudokuBoard very_hard_board {"080006000600000070370591000400000001000000204000200035000800007802004900010059000"};
        static constexpr SudokuBoard impossible_board{"800000000003600000070090200050007000000045700000100030001000068008500010090000400"};
        static constexpr SudokuBoard benchmark_board {"050306007000085024098420603901003206030000010507260908405090380010570002800104070"};

        static std::optional<SudokuBoard> lookupBoard(const std::string board_name) {
            if (board_name == "very_easy_board")  return very_easy_board;
            if (board_name == "easy_board")       return easy_board;
            if (board_name == "medium_board")     return medium_board;
            if (board_name == "very_hard_board")  return very_hard_board;
            if (board_name == "impossible_board") return impossible_board;
            if (board_name == "benchmark_board")  return benchmark_board;
            return std::nullopt;
        }
    };
}