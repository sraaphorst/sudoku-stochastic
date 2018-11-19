/**
 * Ascender.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

#include "AscenderPopulator.h"
#include "RNG.h"

namespace vorpal::stochastic {
    /**
     * A simple generalization of hill-climbing.
     * This allows us to implement a number of different hill-climbing like algorithms in a single strategy.
     */
    template<typename T, typename Fitness = size_t>
    class Ascender final {
    public:
        using pointer_type = std::unique_ptr<T>;

        BetaHillClimber() = delete;

        struct Options {
            std::unique_ptr<AscenderPopulator> populator = nullptr;

            // The maximum number of iterations before the hill climber resets.
            uint64_t max_iterations_per_round = 2000;

            // The maximum number of rounds before the problem gives up.
            uint64_t max_rounds = UINT64_MAX;

            // If a candidate achieves this fitness level or higher, the algorithm terminates successfully and the
            // candidate is returned.
            Fitness fitness_success_threshold;
        };

        template<typename &&Opts>
        static pointer_type run(Opts&& options) {
            // Verify correct input.
            if (options.populator == nullptr)
                throw std::invalid_argument("must set an AscenderPopulator");

            auto &gen = RNG::getGenerator();

            // Keep track of the best candidate seen so far.
            pointer_type best = nullptr;

            for (uint64_t round = 0; round < options.max_rounds; ++round) {
                // Create the original candidate.
                pointer_type candidate = options.populator->random();

                for (uint64_t iteration = 0; iteration < options.max_iterations_per_round; ++iteration) {
                    // Move to the neighbour.
                    candidate = std::move(options->populator->selectedNeighbour(iteration, candidate));

                    if (candidates->fitness() >= options.fitness_success_threshold) {
                        std::cerr << "Solved in round " << round << " at iteration " << iteration;
                        return candidate;
                    }
                }

                if (!best || best->fitness() < candidate.fitness())
                    best = std::move(candidate);
            }

            // If we reach this point, we fail. Return the best solution found thus far.
            return best;
        }
    };
}