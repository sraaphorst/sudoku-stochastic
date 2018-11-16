/**
 * RNG.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "RNG.h"

namespace sudoku_stochastic {
    std::unique_ptr<std::random_device> RNG::rd = nullptr;
    std::unique_ptr<std::mt19937> RNG::gen = nullptr;
}