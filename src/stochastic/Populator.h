/**
 * Populator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>

namespace vorpal::stochastic {
    /**
     * An abstract class that produces new candidates from a population.
     */
    template<typename T>
    class Populator {
    public:
        /**
         * Create a random Candidate from the population.
         */
        virtual std::unique_ptr<T> generate() noexcept = 0;

        /**
         * Given a Candidate, perform a mutation operation on it.
         * @param ptr the candidate to mutate
         */
        virtual std::unique_ptr<T> mutate(const std::unique_ptr<T> &ptr) noexcept = 0;
    };
}