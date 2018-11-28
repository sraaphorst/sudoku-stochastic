/**
 * BetaHillClimbingAlgorithm.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "HillClimbingAlgorithm.h"

namespace vorpal::stochastic {
    template<typename T,
            typename Fitness = size_t,
            HillClimbingOptions<T, Fitness>>
    class BetaHillClimbing final: public HillClimbingAlgorithm<T, Fitness> {
        static_assert(std::is_arithmetic_v<Fitness>);
    public:
        using option_type = HillClimbingOptions<T, Fitness>;
        using pointer_type = std::unique_ptr<T>;

        BetaHillClimbingAlgorithm() = default;

    protected:
        /**
         * We always accept the new state.
         */
        virtual bool accept(const pointer_type&, const pointer_type&, const option_type&, std::unique_ptr<State>&) {
            return true;
        }
    };
}