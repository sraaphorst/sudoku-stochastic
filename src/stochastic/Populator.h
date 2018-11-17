/**
 * Populator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>

#include "Candidate.h"

namespace vorpal::stochastic {
    /**
     * An abstract class that produces new candidates from a population.
     */
    class Populator {
    public:
        /**
         * Create a random Candidate from the population.
         */
        virtual std::unique_ptr<Candidate> generate() noexcept = 0;

        /**
         * Given a Candidate, perform a mutation operation on it.
         * @param ptr the candidate to mutate
         */
        virtual std::unique_ptr<Candidate> mutate(const std::unique_ptr<Candidate> &ptr) noexcept;
    }
}