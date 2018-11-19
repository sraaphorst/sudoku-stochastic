/**
 * PopulationSelector.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <algorithm>
#include <iterator>
#include <memory>
#include <random>
#include <set>
#include <stdexcept>
#include <vector>

#include <boost/format.hpp>

#include "PopulationSelector.h"
#include "RNG.h"

namespace vorpal::stochastic {
    /**
     * A selector takes a generation of candidates and chooses one from the generation based on certain
     * criteria. This is the superclass of all selectors.
     * @tparam T the type represented in the container
     */
    template<typename T>
    struct Selector {
        Selector() = default;
        virtual ~Selector() = default;
        virtual size_t select(const std::vector<std::unique_ptr<T>>&) const = 0;
    };

    /**
     * A simple selector that randomly picks an element.
     */
    template<typename T>
    struct RandomSelector final: Selector<T> {
        RandomSelector() = default;
        ~RandomSelector() = default;
        size_t select(const std::vector<std::unique_ptr<T>> &ps) const override {
            const size_t distance = ps.size();
            std::uniform_int_distribution<uint64_t> distribution(0, distance - 1);
            const size_t selection = distribution(RNG::getGenerator());
            return selection;
        }
    };

    /**
     * A k-tournament selector chooses k elements at random, evaluates their fitness, and returns the best.
     */
    template<typename T>
    class KTournamentSelector final: public Selector<T> {
        size_t k;

    public:
        explicit KTournamentSelector(size_t k): k{k} {
            if (k == 0)
                throw std::invalid_argument("k-tournament selection requires positive k");
        }
        ~KTournamentSelector() = default;

        size_t select(const std::vector<std::unique_ptr<T>> &ps) const override {
            if (k > ps.size())
                throw std::invalid_argument((boost::format("cannot have a %1%-tournament with a population of size %2%")
                                             % k % ps.size()).str());

            std::uniform_int_distribution<uint64_t> distribution(0, ps.size() - 1);
            auto &gen = RNG::getGenerator();

            // We need a set of k unique indices into ps.
            // A shuffle would be a poor way to do this because the size of ps is likely to be much larger than k.
            std::set<size_t> indices;
            while (indices.size() < k) {
                indices.insert(distribution(gen));
            }

            // Now determine the max element in indices with regards to fitness.
            return *std::max_element(std::cbegin(indices), std::cend(indices),
                    [&ps] (const auto idx1, const auto idx2) { return ps[idx1]->fitness() < ps[idx2]->fitness(); });
        }
    };


    /**
     * Perform a roulette selection, i.e. allot everyone a probability commensurate with their fitness.
     */
     template<typename T>
     struct RouletteSelector final: Selector<T> {
         RouletteSelector() = default;
         ~RouletteSelector() = default;

         size_t select(const std::vector<std::unique_ptr<T>> &ps) const override {
             // Tally the fitnesses.
            // We could use std::transform_reduce here, but it seems to only be in llvm and not gcc.
            uint64_t tally = 0;
            for (const auto &p: ps)
                tally += p->fitness();

             std::uniform_int_distribution<uint64_t> distribution(0, tally - 1);
             uint64_t goal = distribution(RNG::getGenerator());

             for (size_t i = 0; i < ps.size(); ++i) {
                 const auto fitness = ps[i]->fitness();
                 if (goal < fitness)
                     return i;
                 goal -= fitness;
             }

             // This point should never be reached.
             return 0;
         }
     };
}