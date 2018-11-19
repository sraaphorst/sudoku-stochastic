/**
 * RNG.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <random>

namespace vorpal::stochastic {
    /**
     * A class that encompasses the random device and Mersenne twister, since while creating distributions is
     * very fast, creating random devices and generators is incredibly slow, so we only want to have to do this once.
     */
    class RNG {
    private:
        static std::unique_ptr<std::mt19937> gen;

    public:
        RNG() = delete;

        /**
         * IMPORTANT: an auto REFERENCE must be taken from this function.
         * If the return value is not a reference, it will make a copy of gen and thus will display the same results
         * repeatedly since gen will never change.
         *
         * auto &gen = RNG::getGenerator();
         */
        static auto &getGenerator() noexcept {
            return *gen;
        }
    };
}


