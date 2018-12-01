/**
 * HillClimbinglgorithm.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "AscenderPopulator.h"
#include "RNG.h"

namespace vorpal::stochastic {
    template<typename T, typename Fitness = size_t>
    struct HillClimbingOptions {
        static_assert(std::is_arithmetic_v<Fitness>);

        using populator_type = AscenderPopulator<T>;

        std::unique_ptr<populator_type> populator = nullptr;

        // The maximum number of iterations before the ascender resets.
        // This is analogous to hill climnbing starting over, or the great deluge deciding to stop.
        uint64_t max_iterations_per_round = 200'000;

        // The maximum number of rounds before the problem gives up.
        uint64_t max_rounds = 1'000'000;

        // If a candidate achieves this fitness level or higher, the algorithm terminates successfully and the
        // candidate is returned.
        Fitness fitness_success_threshold;
    };

    /**
     * A simple generalization of hill-climbing.
     * This allows us to implement a number of different hill-climbing like algorithms in a single strategy.
     */
    template<typename T,
            typename Fitness = size_t,
            typename Options = HillClimbingOptions<T, Fitness>,
            typename State = void*>
    class HillClimbingAlgorithm {
        static_assert(std::is_arithmetic_v<Fitness>);
    public:
        using options_type = Options;
        using pointer_type = std::unique_ptr<T>;

    protected:
        std::unique_ptr<options_type> options;

    public:
        HillClimbingAlgorithm() = default;

        template<typename Opts>
        pointer_type run(Opts &&options) {
            // Verify correct input.
            if (options.populator == nullptr)
                throw std::invalid_argument("must set an AscenderPopulator");

            // Keep track of the best candidate seen so far.
            pointer_type best = nullptr;

            for (uint64_t round = 0; round < options.max_rounds; ++round) {
                // Initialize the state.
                auto state = initState(options);

                // Create the original candidate.
                pointer_type cur = options.populator->generate();

                for (uint64_t iteration = 0; iteration < options.max_iterations_per_round; ++iteration) {
                    // Get the next candidate and determine if we should move to it.
                    pointer_type next = options.populator->generateNeighbour(cur);
                    if (accept(next, cur, options, state))
                        cur = std::move(next);

                    // Check if we are considered a solution.
                    if (cur->fitness() >= options.fitness_success_threshold)
                        return cur;

                    // Check if we are the best seen so far, and if so, copy.
                    if (!best || best->fitness() < cur->fitness()) {
                        best = std::make_unique<T>(*cur);
                        std::cerr << "Best: " << best->fitness() << '\n';
                    }
                }
            }

            // If we reach this point, we fail. Return the best solution found thus far.
            return best;
        }

    protected:
        /**
         * Create an initial state to store data about the heuristic
         * @return the initialized state
         */
        virtual std::unique_ptr<State> initState(const options_type&) {
            return std::make_unique<State>();
        }

        /**
         * Determine if we should accept a next state.
         * Heuristics with more complicated rules should adjust this accordingly and adjust their state.
         * @param next candidate state to accept
         * @param cur current state state
         * @return true to accept the next state, false to reject it
         */
        virtual bool accept(const pointer_type &next, const pointer_type &cur, const options_type&, std::unique_ptr<State>&) {
            return next->fitness() > cur->fitness();
        }
    };
}