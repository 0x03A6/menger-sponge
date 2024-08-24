#pragma once
#ifndef COLOR_HPP
#define COLOR_HPP

#include <graphics.h>
#include <algorithm>

#include "vector.hpp"

struct Color {
    double r;
    double g;
    double b;
    Color() {}
    constexpr Color(double _r, double _g, double _b)
        :r(_r), g(_g), b(_b) {}
    int toNum() const {
        return RGB(b * 255.999, g * 255.999, r * 255.999);
    }

    Color limited() {
        return { std::min(r, 1.0), std::min(g, 1.0), std::min(b, 1.0) };
    }

    Color operator*(const double x) const {
        return { r * x, g * x, b * x };
    }

    Color operator*(const Color c) const {
        return { r * c.r, g * c.g, b * c.b };
    }

    Color operator+(const Color c) const {
        return { r + c.r, g + c.g, b + c.b };
    }

    Color operator/(const int x) const {
        return { r / x, g / x, b / x };
    }

    void operator+=(const Color c) {
        r += c.r;
        g += c.g;
        b += c.b;
    }
    void operator*=(const Color c) {
        r *= c.r;
        g *= c.g;
        b *= c.b;
    }
};

#endif