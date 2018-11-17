/**
 * Candidate.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

namespace vorpal::stochastic {
    /**
     * Abstract superclass representing a member of the population under question.
     */
    struct Candidate {
        virtual size_t fitness() const = 0;
    };
}
