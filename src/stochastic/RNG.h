/**
 * RNG.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <mutex>
#include <random>

namespace vorpal::stochastic {
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
            static std::mutex rd_mutex, gen_mutex;

            // Wrap these in mutexes so that we only write to the variables exactly once.
            {
                std::lock_guard<std::mutex> guard{gen_mutex};
                if (!rd)
                    rd = std::make_unique<std::random_device>();
            }
            {
                std::lock_guard<std::mutex> guard(gen_mutex);
                if (!gen)
                    gen = std::make_unique<std::mt19937>((*rd)());
            }
            return *gen;
        }
    };
}


