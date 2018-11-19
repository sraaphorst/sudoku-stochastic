/**
 * RNG.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <memory>
#include <random>

#include "RNG.h"

namespace vorpal::stochastic {
    std::unique_ptr<std::mt19937> RNG::gen{std::make_unique<std::mt19937>(std::random_device{}())};
}