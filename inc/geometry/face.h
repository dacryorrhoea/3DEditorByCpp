#pragma once

#include <iostream>

struct Face {
    int f1;
    int f2;
    int f3;

    constexpr Face(int a, int b, int c) noexcept
        : f1(a)
        , f2(b)
        , f3(c)
    {}

    constexpr Face operator+(int s) const noexcept {
        return {f1 + s, f2 + s, f3 + s};
    }

    friend std::ostream& operator<<(std::ostream& os, const Face& f) {
        os << f.f1 << ' ' << f.f2 << ' ' << f.f3;
        return os;
    }
};