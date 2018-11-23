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
         * Using this as a general, one-time function allows us to implement a number
         * of different hill-climber / ascender variants via subclasses, such as:
         *
         * 1. Hill climbing, for which this class should be instantiated.
         * 1. Beta hill-climber
         * 2. Tabu search
         * 3. Great deluge
         * 4. Simulated annealing
         *
         * @param the iteration number
         * @param current the current node
         * @return a node that, by the hill-climbing variant strategy, is picked next.
         */
        virtual pointer_type selectedNeighbour(int iteration, pointer_type &current) const = 0;
    };
}