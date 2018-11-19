/**
 * GeneticCandidate.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>

#include "Candidate.h"

namespace vorpal::stochastic {
    /**
     * A Populator is the most basic subclass for creating "population" (i.e. \see{Candidte}) for heuristics.
     * Other heuristics will require specific subclasses of Populator.
     */
    template<typename T>
    class Populator {
    protected:
        Populator() = default;

    public:
        virtual ~Populator() = default;

        using pointer_type = std::unique_ptr<T>;

        /**
         * Create a random Candidate from the population.
         * @return the random candidate
         */
        virtual pointer_type generate() noexcept = 0;
    };
}