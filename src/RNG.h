/**
 * RNG.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <random>

namespace sudoku_stochastic {
    /**
     * A class that encompasses the random device and Mersenne twister, since while creating distributions is
     * very fast, creating random devices and generators is incredibly slow, so we only want to have to do this once.
     */
    class RNG {
    private:
        static std::unique_ptr<std::random_device> rd;
        static std::unique_ptr<std::mt19937> gen;

    public:
        RNG() = delete;

        static auto &getGenerator() noexcept {
            if (!rd)
                rd = std::make_unique<std::random_device>();
            if (!gen)
                gen = std::make_unique<std::mt19937>((*rd)());
            return *gen;
        }
    };
}


