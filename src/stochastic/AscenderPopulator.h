/**
 * AscenderPopulator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "Populator.h"

namespace vorpal::stochastic {
    template<typename T>
    class AscenderPopulator: public Populator<T> {
    protected:
        AscenderPopulator() = default;

    public:
        virtual ~AscenderPopulator() = default;

        using pointer_type = std::unique_ptr<T>;

        /**
         * Given the current node on the specified round, select one of its neighbours.
         * @param the iteration number
         * @param current the current node
         * @return a neighbour of the current node
         */
        virtual pointer_type generateNeighbour(pointer_type &current) = 0;
    };
}