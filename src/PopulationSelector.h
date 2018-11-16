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

namespace sudoku_stochastic::genetic {
    /**
     * Perform a k-tournament selection, i.e. pick k population members randomly and return the most fit.
     * @return the most fit out of a k-sample
     */
    template<typename Iter, typename Fitness>
    Iter tournamentSelection(size_t k, Iter begin, Iter end, Fitness f) {
        const auto total = static_cast<unsigned long long>(std::distance(begin, end));
        if (total < k)
            return end;

        // This is irritatingly complicated because we want an Iter, and not its value type.
        // Thus, we simply keep trying to get k elements.
        std::uniform_int_distribution<uint64_t > distribution(0, total - 1);
        auto &gen = RNG::getGenerator();
//        std::random_device rd;
//        std::mt19937 gen(rd());

        std::set<size_t> indices{};
        while (indices.size() < k)
            indices.insert(distribution(gen));

        auto idx = *std::max_element(std::cbegin(indices), std::cend(indices),
                [&f, begin](const auto i1, const auto i2) {
            auto begin1 = begin;
            std::advance(begin1, i1);
            auto begin2 = begin;
            std::advance(begin2, i2);
            return f(*begin1) < f(*begin2);
        });

        std::advance(begin, idx);
        return begin;
    }

    /**
     * Perform a roulette selection, i.e. allot everyoe a probability commensurate with their fitness.
     * @return a fitness weighted random choice
     */
    template<typename Iter, typename OutIter, typename Fitness>
    Iter rouletteSelection(size_t k, Iter begin, Iter end, OutIter outbegin, Fitness f) {
        if (std::distance(end, begin) < k)
            return end;

        // Tally all the fitnesses.
        const auto total = std::transform_reduce(begin, end, uint64_t{0}, f, std::plus<>());

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint64_t > distribution(0, total);
        uint64_t value = distribution(rd);

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
    template<typename Iter, typename OutIter>
    Iter randomSelection(size_t k, Iter begin, Iter end) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint64_t > distribution(0, static_cast<unsigned long long>(std::distance(end, begin)));
        return begin + distribution(gen);
    }
}