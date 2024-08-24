#pragma once
#ifndef SURFECE_HPP
#define SURFECE_HPP
#include "vector.hpp"

//平面类
struct Surface {
    double a, b, c, d;        //ax + by + cz + d = 0
    Surface() {}
    //给定平面法向量和一个面内的点，用于无限延伸的平面
    Surface(V3d normal_vec, V3d p) {
        a = normal_vec.x;
        b = normal_vec.y;
        c = normal_vec.z;
        d = -(a * p.x + b * p.y + c * p.z);
    }
    constexpr Surface(double _a, double _b, double _c, double _d)
        : a(_a), b(_b), c(_c), d(_d) {}
    V3d *normal() {
        return (V3d *)&a;
    }
};
#endif