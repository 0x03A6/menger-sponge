#pragma once
#ifndef V3d_HPP
#define V3d_HPP

#include <cmath>

template<typename T>
struct Vector3D {
    T x, y, z;
    constexpr Vector3D(T _x, T _y, T _z) :x(_x), y(_y), z(_z) {}
    Vector3D() {}
    //求向量的模长
    T len() {
        return sqrt((T)x * x + y * y + z * z);
    }
    //获得与此向量角度相同的单位向量
    void normalize() {
        const T l = len();
        x = x / l;
        y = y / l;
        z = z / l;
    }
    Vector3D normalized() const {
        const T l = len();
        return { x / l, y / l, z / l };
    }
    //获得与此向量模长相等、方向相反的向量
    Vector3D reverse() {
        return { -x, -y, -z };
    }
    //将此向量向v投影，返回投影长度
    T project(Vector3D);

    Vector3D operator + (Vector3D v) const {
        return { x + v.x, y + v.y, z + v.z };
    }
    void operator += (Vector3D v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }
    Vector3D operator - (Vector3D v) const {
        return { x - v.x, y - v.y, z - v.z };
    }
    void operator -= (Vector3D v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }
    Vector3D operator * (T k) const {
        return { x * k, y * k, z * k };
    }
};

template<typename T>
Vector3D<T> operator * (T k, Vector3D<T> v) {
    return { v.x * k, v.y * k, v.z * k };
}

//点乘
template<typename T>
T dot(Vector3D<T> a, Vector3D<T> b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

//叉乘
template<typename T>
Vector3D<T> cross(Vector3D<T> a, Vector3D<T> b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

template<typename T>
T Vector3D<T>::project(Vector3D<T> v) {
    return dot(*this, v) / v.len();
}

//向量夹角
template<typename T>
T angle(Vector3D<T> p, Vector3D<T> q) {
    return acos(dot(p, q) / (p.len() * q.len()));
}

template<typename T>
inline Vector3D<T> normalize(const Vector3D<T> v) {
    const T len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return { v.x / len, v.y / len, v.z / len };
}

template<typename T>
struct Vector2D {
    T x, y;
    Vector2D(T _x, T _y) :x(_x), y(_y) {}
    Vector2D() {}
    //求向量的模长
    T len() const {
        return sqrt(x * x + y * y);
    }
    //获得与此向量角度相同的单位向量
    Vector2D normalize() {
        const T l = len();
        return { x / l, y / l };
    }
    Vector2D normalized() const {
        const T l = len();
        return { x / l, y / l };
    }
    //获得与此向量模长相等、方向相反的向量
    Vector2D reverse() {
        return { -x, -y };
    }
    //将此向量向v投影，返回投影长度
    T project(Vector2D);

    Vector2D operator + (Vector2D v) const {
        return { x + v.x, y + v.y };
    }
    void operator += (Vector2D v) {
        x += v.x;
        y += v.y;
    }
    Vector2D operator - (Vector2D v) const {
        return { x - v.x, y - v.y };
    }
    void operator -= (Vector2D v) {
        x -= v.x;
        y -= v.y;
    }
    Vector2D operator * (T k) const {
        return { x * k, y * k };
    }
};

template<typename T>
Vector2D<T> operator * (T k, Vector2D<T> v) {
    return { v.x * k, v.y * k };
}

//点乘
template<typename T>
T dot(Vector2D<T> a, Vector2D<T> b) {
    return a.x * b.x + a.y * b.y;
}

template<typename T>
T Vector2D<T>::project(Vector2D<T> v) {
    return dot(*this, v) / v.len();
}

//向量夹角
template<typename T>
T angle(Vector2D<T> p, Vector2D<T> q) {
    return acos(dot(p, q) / (p.len() * q.len()));
}

//叉乘Z坐标
template<typename T>
T crossZ(Vector2D<T> p, Vector2D<T> q) {
    return p.x * q.y - p.y * q.x;
}

struct Pixel {
    int l;  // line
    int c;  // column
};

typedef Vector3D<double> V3d;
typedef Vector2D<double> V2d;

#endif