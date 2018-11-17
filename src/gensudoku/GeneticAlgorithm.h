/**
 * GeneticAlgorithm.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <vector>

#include "Populator.h"
#include "Candidate.h"

namespace vorpal::stochastic {
    /**
     * The fundamentals of running the genetic algorithm on the problem.
     * @tparam Fitness the measure of fitness for a candidate
     */
    template<typename T, typename Fitness=double>
    class GeneticAlgorithm {
    public:
        using pointer_type = std::unique_ptr<T>;

        GeneticAlgorithm() = delete;

        class Options {
            Populator<T> populator;
            size_t population_size;
            double mutation_probability;
            double crossover_probability;
            size_t max_generations;
            Fitness fitness_success_threshold;
        };

        template<typename Opts>
        public static Candidate<T> run(Opts&& options) {
            std::vector<pointer_type> prevGeneration{options.population_size};
            for (int i = 0; i < options.population_size; ++i)
                prevGeneration[i] = std::move(options.populator.generate());
            return prevGeneration[0];
        }
    };
}


