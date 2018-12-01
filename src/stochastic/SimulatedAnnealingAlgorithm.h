/**
 * SimulatedAnnealingAlgorithm.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cmath>
#include <random>

#include "HillClimbingAlgorithm.h"
#include "RNG.h"

namespace vorpal::stochastic {
    template<typename T, typename Fitness = size_t>
    struct SimulatedAnnealingOptions: HillClimbingOptions<T, Fitness> {
        static_assert(std::is_arithmetic_v<Fitness>);

        double initial_temperature;
        double alpha;
    };

    namespace details {
        struct SimulatedAnnealingState {
            double temperature;
        };
    }

    template<typename T, typename Fitness = size_t>
    class SimulatedAnnealingAlgorithm final: public HillClimbingAlgorithm<
            T,
            Fitness,
            SimulatedAnnealingOptions<T, Fitness>,
            details::SimulatedAnnealingState
            > {
        static_assert(std::is_arithmetic_v<Fitness>);
        using state_type = details::SimulatedAnnealingState;

    public:
        using options_type = SimulatedAnnealingOptions<T, Fitness>;
        using pointer_type = std::unique_ptr<T>;

        SimulatedAnnealingAlgorithm() = default;

    protected:
        /**
         * Create a new state with the initial temperature.
         */
        std::unique_ptr<state_type> initState(const options_type &options) override {
            auto state = std::make_unique<state_type>();
            state->temperature = options.initial_temperature;
            return state;
        }

        /**
         * In Simulated Annealing, we always accept better answers.
         * We accept worse answers with a constantly decreasing probability.
         */
        bool accept(const pointer_type &next, const pointer_type &cur,
                const options_type &options, std::unique_ptr<state_type> &state) override {
            auto &gen = RNG::getGenerator();
            std::uniform_real_distribution dist;

            // Always accept if we see improvement; if not, accept with certain probability.
            const auto e = cur->fitness();
            const auto ep = next->fitness();
            const auto threshold = ep > e ? 1.0 : std::exp(-1.0*(e - ep)/state->temperature);

            // Adjust the temperature and return the result.
            state->temperature *= options.alpha;
            return threshold >= dist(gen);
        }
    };
}