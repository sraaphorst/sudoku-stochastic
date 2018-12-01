/**
 * GreatDelugeAlgorithm.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <type_traits>

#include "HillClimbingAlgorithm.h"

namespace vorpal::stochastic {
    template<typename T,
            typename W = double,
            typename Fitness = size_t>
    struct GreatDelugeOptions: HillClimbingOptions<T, Fitness> {
        static_assert(std::is_arithmetic_v<W>);
        W initial_water_level;
        W rain_speed;
    };

    namespace details {
        template<typename W = double>
        struct GreatDelugeState {
            static_assert(std::is_arithmetic_v<W>);
            W water_level;
        };
    }

    template<typename T,
            typename W = double,
            typename Fitness = size_t>
    class GreatDelugeAlgorithm final: public HillClimbingAlgorithm<
            T,
            Fitness,
            GreatDelugeOptions<T, W, Fitness>,
            details::GreatDelugeState<W>
            > {
        static_assert(std::is_arithmetic_v<Fitness>);
        static_assert(std::is_arithmetic_v<W>);
        using state_type = details::GreatDelugeState<W>;

    public:
        using options_type = GreatDelugeOptions<T, W, Fitness>;
        using pointer_type = std::unique_ptr<T>;

        GreatDelugeAlgorithm() = default;

    protected:
        /**
         * Create a new state with the water level initialized from the options.
         */
        std::unique_ptr<state_type> initState(const options_type &options) override {
            auto state = std::make_unique<state_type>();
            state->water_level = options.initial_water_level;
            return state;
        }

        /**
         * In the Great Deluge, we proceed as long as we are still above the water level.
         * If we proceed, it rains and the water level increases.
         */
        bool accept(const pointer_type &next, const pointer_type&,
                const options_type &options, std::unique_ptr<state_type> &state) override {
            if (next->fitness() > state->water_level) {
                state->water_level += options.rain_speed;
                return true;
            }
            return false;
        }
    };
}