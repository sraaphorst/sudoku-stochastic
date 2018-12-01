/**
 * TabuSearchAlgorithm.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <deque>
#include <iterator>
#include <memory>
#include <type_traits>

#include "HillClimbingAlgorithm.h"

namespace vorpal::stochastic {
    template<typename T,
            typename Fitness>
    struct TabuSearchOptions : HillClimbingOptions<T, Fitness> {
        size_t tabu_list_length;
    };

    namespace details {
        template<typename T>
        struct TabuSearchState {
            std::deque<T> tabu_list;
        };
    }

    template<typename T, typename Fitness = size_t>
    class TabuSearchAlgorithm final: public HillClimbingAlgorithm<
            T,
            Fitness,
            TabuSearchOptions<T, Fitness>,
            details::TabuSearchState<T>
            > {
        static_assert(std::is_arithmetic_v<Fitness>);
        using state_type = details::TabuSearchState<T>;

    public:
        using options_type = TabuSearchOptions<T, Fitness>;
        using pointer_type = std::unique_ptr<T>;

        TabuSearchAlgorithm() = default;

    protected:
        /**
         * In the Great Deluge, we proceed as long as we are still above the water level.
         * If we proceed, it rains and the water level increases.
         */
        bool accept(const pointer_type &next, const pointer_type &cur,
                const options_type &options, std::unique_ptr<state_type> &state) override {
            // We only allow improvements in tabu search.
            if (next->fitness() < cur->fitness())
                return false;

            // Check if next is in the tabu list.
            const auto iter = std::find(std::cbegin(state->tabu_list), std::cend(state->tabu_list), *next);
            if (iter != std::cend(state->tabu_list))
                return false;

            // Check if we need to pop something from the list.
            while (state->tabu_list.size() >= options.tabu_list_length)
                state->tabu_list.pop_back();
            state->tabu_list.emplace_front(*cur);
            return true;
        }
    };
}