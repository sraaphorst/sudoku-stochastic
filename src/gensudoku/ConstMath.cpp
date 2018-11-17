/**
 * ConstMath.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cstddef>

#include "ConstMath.h"

namespace vorpal::gensudoku {
    constexpr unsigned char cToUpper(unsigned char c) {
        return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
    }

    constexpr size_t fromBase36(unsigned char c) {
        const char uc = cToUpper(c);
        if (uc >= '0' && uc <= '9')
            return static_cast<size_t>(uc - '0');
        else if (uc >= 'A' && uc <= 'Z')
            return static_cast<size_t>(uc - 'A') + 10;
        else
            return static_cast<size_t>(uc);
    }

    constexpr unsigned char toBase36(size_t d) {
        assert(d >= 0 && d < 36);
        if (d < 10)
            return static_cast<unsigned char>('0' + d);
        else
            return static_cast<unsigned char>(d - 10 + 'A');
    }
}