/**
 * GeneticPopulator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>

#include "Populator.h"

namespace vorpal::stochastic {
    /**
     * An abstract class that produces new candidates from a population.
     */
    template<typename T>
    class GeneticPopulator: public Populator<T> {
    protected:
        GeneticPopulator() = default;

    public:
        virtual ~GeneticPopulator() = default;

        using pointer_type = std::unique_ptr<T>;

        /**
         * Given a Candidate, perform a mutation operation on it.
         * @param ptr the candidate to mutate
         * @return the mutated candidate
         */
        virtual pointer_type mutate(const pointer_type &ptr) noexcept = 0;

        /**
         * Given two parents, crossover-breed them together to produce a set of two offspring.
         * @param p0 first parent
         * @param p1 second parent
         * @return the two offspring
         */
        virtual std::pair<pointer_type, pointer_type> crossover(const pointer_type &p0, const pointer_type &p1) noexcept = 0;

        /**
         * Duplicate contents of a candidate to another unique_ptr to indicate survival.
         * @param the surviving element
         * @return a copy of it for the next generation
         */
         virtual pointer_type survive(const pointer_type &p) noexcept = 0;
    };
}