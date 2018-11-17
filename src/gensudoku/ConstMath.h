/**
 * constmath.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cassert>
#include <cstddef>

namespace vorpal::gensudoku {
    /// constexpr toupper
    constexpr unsigned char cToUpper(unsigned char c) {
        return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
    }

    /// Convert from a char in base 36 (0-9, A-Z).
    constexpr size_t fromBase36(unsigned char c) {
        const char uc = cToUpper(c);
        if (uc >= '0' && uc <= '9')
            return static_cast<size_t>(uc - '0');
        else if (uc >= 'A' && uc <= 'Z')
            return static_cast<size_t>(uc - 'A') + 10;
        else
            return static_cast<size_t>(uc);
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
