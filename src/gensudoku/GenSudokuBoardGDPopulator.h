/**
 * GenSudokuBoardGDPopulator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "GenSudokuBoardHCPopulator.h"

namespace vorpal::gensudoku {
    template<size_t N,
            const auto NN = N * N,
            const auto BoardSize = NN * NN>
    class GenSudokuBoardGDPopulator final: public GenSudokuBoardHCPopulator<N> {
        // We allow these to be doubles so that we can work in units smaller than integral if we want.
        double water_level;
        double rain_speed;

    public:
        __GENSUDOKUBOARD_POPULATOR_GENERIC_CONSTRUCTORS(GenSudokuBoardGDPopulator)

        GenSudokuBoardGDPopulator(const data_type &partial_board, double water_level, double rain_speed):
            GenSudokuBoardHCPopulator<N>{partial_board}, water_level(water_level), rain_speed{rain_speed} {}

        virtual pointer_type selectedNeighbour(int, pointer_type &current) override {
            auto neighbour = std::move(this->nOperator(current));

            if (neighbour->fitness() > water_level) {
                water_level += rain_speed;
                //std::cerr << "Water=" << water_level << ", rain=" << rain_speed << '\n';
                return std::move(neighbour);
            } else
                return std::move(current);
        }
    };

    using SudokuBoardGDPopulator = GenSudokuBoardGDPopulator<3>;
}