#pragma once

#ifndef SPACE_CONVERTER_HPP
#define SPACE_CONVERTER_HPP

#include "vector.hpp"

// 为了处理方便定义了用于坐标轴交换的三个类。
// 有点抽象。
template<typename T>
struct XYZ : public Vector3D<T> {
    T &X() { return Vector3D<T>::x; }
    T &Y() { return Vector3D<T>::y; }
    T &Z() { return Vector3D<T>::z; }
    XYZ() {}
    XYZ(V3d a) :Vector3D<T>(a) {}   //ATTENTION
    XYZ(const T x, const T y, const T z) {
        X() = x; Y() = y; Z() = z;
    }
};
template<typename T>
struct YXZ : public Vector3D<T> {
    T &X() { return Vector3D<T>::y; }
    T &Y() { return Vector3D<T>::x; }
    T &Z() { return Vector3D<T>::z; }
    YXZ() {}
    YXZ(V3d a) :Vector3D<T>(a) {}   //ATTENTION
    YXZ(const T x, const T y, const T z) {
        X() = x; Y() = y; Z() = z;
    }
};
template<typename T>
struct ZXY : public Vector3D<T> {
    T &X() { return Vector3D<T>::z; }
    T &Y() { return Vector3D<T>::x; }
    T &Z() { return Vector3D<T>::y; }
    ZXY() {}
    ZXY(V3d a) :Vector3D<T>(a) {}   //ATTENTION
    ZXY(const T x, const T y, const T z) {
        X() = x; Y() = y; Z() = z;
    }
};

using dXYZ = XYZ<double>;
using dYXZ = YXZ<double>;
using dZXY = ZXY<double>;

#endif