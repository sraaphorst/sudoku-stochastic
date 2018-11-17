/**
 * RNG.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <memory>
#include <random>

#include "RNG.h"

namespace vorpal::stochastic {
    std::unique_ptr<std::random_device> RNG::rd = nullptr;
    std::unique_ptr<std::mt19937> RNG::gen = nullptr;
}