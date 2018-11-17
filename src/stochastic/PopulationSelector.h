/**
 * PopulationSelector.h
 *
 * By Sebastian Raaphorst, 2018.
 */


#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <set>
#include <vector>

#include "PopulationSelector.h"
#include "RNG.h"

namespace vorpal::stochastic {
    /**
     * Perform a k-tournament selection, i.e. pick k population members randomly and return the most fit.
     * @return the most fit out of a k-sample
     */
    template<typename Iter>
    Iter tournamentSelection(size_t k, Iter begin, Iter end) {
        const auto total = static_cast<unsigned long long>(std::distance(begin, end));
        if (total < k)
            return end;

        // This is irritatingly complicated because we want an Iter, and not its value type.
        // Thus, we simply keep trying to get k elements.
        std::uniform_int_distribution<uint64_t > distribution(0, total - 1);
        auto &gen = RNG::getGenerator();

        std::set<size_t> indices{};
        while (indices.size() < k)
            indices.insert(distribution(gen));

        auto idx = *std::max_element(std::cbegin(indices), std::cend(indices),
                [begin](const auto i1, const auto i2) {
            auto begin1 = begin;
            std::advance(begin1, i1);
            auto begin2 = begin;
            std::advance(begin2, i2);
            return (*begin1)->fitness() < (*begin2)->fitness();
        });

        std::advance(begin, idx);
        return begin;
    }

    /**
     * Perform a roulette selection, i.e. allot everyoe a probability commensurate with their fitness.
     * @return a fitness weighted random choice
     */
    template<typename Iter>
    Iter rouletteSelection(size_t k, Iter begin, Iter end) {
        if (std::distance(end, begin) < k)
            return end;

        // Tally all the fitnesses.
        const auto total = std::transform_reduce(begin, end, uint64_t{0},
                [](const auto &a) { return (*a)->fitness(); }, std::plus<>());

        std::uniform_int_distribution<uint64_t > distribution(0, total);
        uint64_t value = distribution(RNG::getGenerator());

        // Find the selection.
        while (begin != end) {
            const auto currValue = f(begin);
            if (currValue <= value)
                return begin;
            value -= currValue;
        }

        // We should never get here.
        return end;
    }

    /**
     * Perform a completely random selection.
     * @return a random element
     */
    template<typename Iter>
    Iter randomSelection(size_t k, Iter begin, Iter end) {
        std::uniform_int_distribution<uint64_t > distribution(0, static_cast<unsigned long long>(std::distance(end, begin)));
        return begin + distribution(RNG::getGenerator());
    }
}