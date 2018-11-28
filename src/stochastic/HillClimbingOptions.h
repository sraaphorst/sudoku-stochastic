/**
 * HillClimbingOptions.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>

#include "HillClimbingPopulator.h"

namespace vorpal::stochastic {
    template<typename T, typename Fitness = size_t>
    struct HillClimbingOptions {
        static_assert(std::is_arithmetic_v<Fitness>);

        using populator_type = HillClimbingPopulator<T>;

        std::unique_ptr<populator_type> populator = nullptr;

        // The maximum number of iterations before the ascender resets.
        // This is analogous to hill climnbing starting over, or the great deluge deciding to stop.
        uint64_t max_iterations_per_round = 200'000;

        // The maximum number of rounds before the problem gives up.
        uint64_t max_rounds = UINT64_MAX;

        // If a candidate achieves this fitness level or higher, the algorithm terminates successfully and the
        // candidate is returned.
        Fitness fitness_success_threshold;
    };
}