/**
 * GeneticAlgorithm.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <omp.h>

#include <memory>
#include <vector>

#include "Candidate.h"
#include "Populator.h"
#include "PopulationSelector.h"
#include "RNG.h"

namespace vorpal::stochastic {
    /**
     * The fundamentals of running the genetic algorithm on the problem.
     * @tparam Fitness the measure of fitness for a candidate
     */
    template<typename T, typename Fitness=double>
    class GeneticAlgorithm final {
    public:
        using pointer_type = std::unique_ptr<T>;

        GeneticAlgorithm() = delete;

        struct Options {
            std::unique_ptr<Selector<T>> selector;
            std::unique_ptr<Populator<T>> populator;
            size_t population_size;
            size_t min_populator_size;
            double mutation_probability;
            double crossover_probability;
            size_t max_generations;
            Fitness fitness_success_threshold;
        };

        template<typename Opts>
        static pointer_type run(Opts&& options) {
            // Force population_size to be even for convenience, and ensure it is big enough for selection.
            assert(options.population_size % 2 == 0);
            assert(options.min_populator_size <= options.population_size);

            auto &gen = RNG::getGenerator();
            std::uniform_real_distribution<double> probabilityGenerator;

            std::clog << "*** Initializing population." << std::endl;
            std::vector<pointer_type> prevGeneration{options.population_size};
            //#pragma omp parallel for shared(prevGeneration, probabilityGenerator, gen)
            for (int i = 0; i < options.population_size; ++i)
                prevGeneration[i] = std::move(options.populator->generate());
            std::clog << "*** Initialization complete. Fitnesses are:" << std::endl;

            for (size_t i = 0; i < options.population_size; ++i) {
                std::clog << "\ti=" << i << ", fitness=" << prevGeneration[i]->fitness()  << std::endl;
            }

            for (size_t generation = 0; generation < options.max_generations; ++generation) {
                std::clog << "*** Generation " << generation << " beginning. ***" << std::endl;

                // Create the candidates for the next generation.
                std::vector<pointer_type> nextGeneration{};

                //#pragma omp parallel for shared(prevGeneration, probabilityGenerator, gen)
                for (int i = 0; i < options.population_size; i += 2) {
                    // Crossover if probability dictates.
                    if (probabilityGenerator(gen) < options.crossover_probability) {
                        std::clog <<" Crossing over positions " << i << " and " << i + 1 << std::endl;
                        const size_t p0Idx = options.selector->select(prevGeneration);
                        std::clog << "\tChose p0Idx=" << p0Idx << std::endl;
                        const auto &p0 = prevGeneration[p0Idx];
                        std::clog << "\tparent0 has fitness: " << p0->fitness() << std::endl;
                        const size_t p1Idx = options.selector->select(prevGeneration);
                        for (int x = 0; x < 9; ++x) {
                            for (int y = 0; y < 9; ++y)
                                std::clog << (*p0)[x * 9 + y] << ' ';
                            std::clog << std::endl;
                        }

                        std::clog << "\tChose p1Idx=" << p1Idx << std::endl;
                        const auto &p1 = prevGeneration[p1Idx];
                        std::clog << "\tparent1 has fitness: " << p1->fitness() << std::endl;
                        for (int x = 0; x < 9; ++x) {
                            for (int y = 0; y < 9; ++y)
                                std::clog << (*p1)[x * 9 + y] << ' ';
                            std::clog << std::endl;
                        }

                        auto [c0, c1] = options.populator->crossover(p0, p1);
                        std::clog << "\tchild0 has fitness " << c0->fitness() << std::endl;
                        for (int x = 0; x < 9; ++x) {
                            for (int y = 0; y < 9; ++y)
                                std::clog << (*c0)[x * 9 + y] << ' ';
                            std::clog << std::endl;
                        }
                        std::clog << "\tchild1 has fitness " << c1->fitness() << std::endl;
                        for (int x = 0; x < 9; ++x) {
                            for (int y = 0; y < 9; ++y)
                                std::clog << (*c1)[x * 9 + y] << ' ';
                            std::clog << std::endl;
                        }

                        if (probabilityGenerator(gen) < options.mutation_probability) {
                            std::clog << "\tMutating child0" << std::endl;
                            auto m0 = options.populator->mutate(c0);
                            std::clog << "\tchild0 now has fitness " << m0->fitness() << std::endl;
                            nextGeneration.emplace_back(std::move(m0));
                            std::clog << "\tCopied." << std::endl;
                        } else {
                            std::clog << "\tCopying child0" << std::endl;
                            nextGeneration.emplace_back(std::move(c0));
                            std::clog << "\tCopied." << std::endl;
                        }
                        if (probabilityGenerator(gen) < options.mutation_probability) {
                            std::clog << "\tMutating child1" << std::endl;
                            auto m1 = options.populator->mutate(c1);
                            std::clog << "\tchild0 now has fitness " << m1->fitness() << std::endl;
                            nextGeneration.emplace_back(std::move(m1));
                            std::clog << "\tCopied." << std::endl;
                        } else {
                            std::clog << "\tCopying child1" << std::endl;
                            nextGeneration.emplace_back(std::move(c1));
                            std::clog << "\tCopied." << std::endl;
                        }

                    } else {
                        std::clog << "Copying parents " << i << " and " << i+1 << std::endl;
                        auto d0 = options.populator->survive(prevGeneration[i]);
                        auto d1 = options.populator->survive(prevGeneration[i+1]);
                        nextGeneration.emplace_back(std::move(d0));
                        nextGeneration.emplace_back(std::move(d1));
                        std::clog << "\tCopied." << std::endl;
                    }
                }

                // Now get the fittest solution and see if it is fit enough.
//                const auto &fittest = std::max_element(std::cbegin(nextGeneration), std::cend(nextGeneration),
//                        [](const auto &s1, const auto &s2) { return s1->fitness() < s2->fitness(); });
//                if ((*fittest)->fitness() >= options.fitness_success_threshold) {
//                    //const pointer_type fit = std::move(*fittest);
//                    return *fittest;
//                }
                // std::max_element was giving us problems.
                std::clog << "\nDone processing next generation" << std::endl;
                std::clog << "Looking for highest fitness" << std::endl;
                size_t max = 0;
                for (size_t i = 1; i < options.population_size; ++i) {
                    std::cerr << "max=" << max << ", i=" << i  << '\n';
                    std::cerr << "i=" << i << " has fitness " << nextGeneration[i]->fitness() << '\n';
                    if (nextGeneration[i]->fitness() > nextGeneration[max]->fitness()) {
                        std::cerr << "\tbest new fitness: " << max << '\n';
                        std::cerr << "\tfitness: " << nextGeneration[i]->fitness() << '\n';
                        max = i;
                    }
                }
                if (nextGeneration[max]->fitness() > options.fitness_success_threshold)
                    return std::move(nextGeneration[max]);

                // Copy the new generation to the old.
                prevGeneration = std::move(nextGeneration);
            }

            // Too many iterations: fail.
            return nullptr;
        }
    };
}


