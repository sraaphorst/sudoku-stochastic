/**
 * GreatDelugeOptions.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "HillClimbingOptions.h"

namespace vorpal::stochastic {
    template<typename T,
            typename W = double,
            typename Fitness = size_t>
    struct GreatDelugeOptions: HillClimbingOptions<T, Fitness> {
        static_assert(std::is_arithmetic_v<W>);
        W initial_water_level;
        W rain_speed;
    };
}