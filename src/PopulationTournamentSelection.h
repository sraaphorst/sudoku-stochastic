/**
 * PopulationTournamentSelection.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iterator>
#include <random>
#include <vector>

#include "PopulationSelector.h"

namespace sudoku_stochastic::selections {
    /**
     * Perform a k-tournament selection, i.e. pick k population members randomly and return the most fit.
     * @return the most fit out of a k-sample
     */
    template<typename Iter, typename OutIter, typename Fitness>
    Iter tournamentSelection(size_t k, Iter begin, Iter end, OutIter outbegin, Fitness f) {
        if (std::distance(end, begin) < k)
            return end;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::sample(begin, end, outbegin, k, gen);
        return std::max_element(outbegin, outbegin + k, f);
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