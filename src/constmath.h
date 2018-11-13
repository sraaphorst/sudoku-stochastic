/**
 * constmath.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cassert>
#include <cstddef>

namespace sudoku_stochastic::math {
    /// constexpr toupper
    constexpr unsigned char cToUpper(unsigned char c) {
        return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
    }

    /// Convert from a char in base 36 (0-9, A-Z).
    constexpr size_t fromBase36(unsigned char c) {
        const char uc = cToUpper(c);
        if (c >= '0' && c <= '9')
            return c - '0';
        else if (c >= 'A' && c <= 'Z')
            return static_cast<size_t>(10 + c - 'A');
        else
            return c;
    }

    /// Convert from a base 36 digit to a char.
    constexpr unsigned char toBase36(size_t d) {
        assert(d >= 0 && d < 36);
        if (d < 10)
            return static_cast<unsigned char>('0' + d);
        else
            return static_cast<unsigned char>(d - 10 + 'A');
    }
}
