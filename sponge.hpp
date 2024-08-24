#pragma once
#ifndef SPONGE_HPP
#define SPONGE_HPP

#include <algorithm>
#include <random>

#include "vector.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "stack.hpp"
#include "space_converter.hpp"

#define INF (__builtin_inff())

constexpr int W = 800;
constexpr int H = 600;
constexpr V3d INIT_CAMERA_FRONT (-1.01, -1.02, -1.01);
constexpr V3d INIT_CAMERA_HEAD  (-1, 0, 1);
constexpr V3d INIT_CAMERA_POS   (2.01, 2.02, 2.03);
constexpr double CAMERA_AXIS = 3;
constexpr double INIT_CAMERA_VELOCITY = 0.5;
constexpr int FRACTAL_MAX_ITER = 15;
constexpr int SCALE = 300;
constexpr double REFLECT_PROBABILITY = 0.9;
constexpr Color SKY_COLOR(1.0, 1.0, 1.0);
constexpr double DIST_EPS = 1e-8;  // 小于这个数的算作距离的计算误差
constexpr double SPONGE_ROUGHNESS = 1.0;    // 海绵的粗糙程度（和正规定义不太一样，详见 deflectRay）

// 这些是另一个视角的相机设置
// constexpr V3d INIT_CAMERA_FRONT (-1, 0, -0);
// constexpr V3d INIT_CAMERA_HEAD  (0, 0, 1);
// constexpr V3d INIT_CAMERA_POS   (11.0 / 18.0 - 0.00001, 1.0 / 6.0 - 0.0001, 1.0 / 6.0 - 0.000001);

// random number generator
std::mt19937 rng;

// 三组棱分别与三个坐标轴平行的正方体
struct Cube {
    V3d vertex; // 三个坐标值都最小的顶点
    double a; // 边长
    Vector3D<int> pos;    //在“父节点”立方体中的位置
};

bool sgn(double x) {
    if (x > 0)  return true;
    return false;
}

//描述了门格海绵长啥样，pos 代表的单元是实心还是空心，pos 从 0 开始。
bool inSponge(Vector3D<int> pos) {
    //return false;   //DEBUG
    if (pos.x == 0 || pos.x == 2)
        return (pos.y != 1 || pos.z != 1);
    //else
        return (pos.y != 1 && pos.z != 1);
}

/// @brief 判断光线会（先）撞到两个平行的面中的哪个面
/// @param a1 第一个面的法向量方向上的坐标的相对值（对于平行于坐标轴的面，“相对值”直接用法向量对应坐标轴的坐标数值即可）
/// @param a2 第二个面……的相对值（同上）
/// @param origin 光线起点在法向量方向上的相对值
/// @param direction 光线方向在法向量方向的相对值
/// @return 如果是 `true`，则会（先）撞到 `a1` 对应的面，`false` 则 `a2`。如果都撞不上就随便返回一个
bool judgeHit(double a1, double a2, double origin, double direction) {
    const bool close_to_a1 = (std::abs(origin - a1) < DIST_EPS);
    const bool close_to_a2 = (std::abs(origin - a2) < DIST_EPS);
    if (close_to_a1 || close_to_a2)
        return close_to_a2;    // 如果 origin 在其中一个面上，忽略 origin 在的面。
    if ((origin > a1) == (origin > a2)) //如果 a1 与 a2 在 origin 同侧
        return (direction > 0) ^ (a1 > a2); //直接找 direction 方向较近的一个
    return (direction > 0) == (a1 > origin);    //在异侧 找 direction 方向面在 origin 前方的一个
}

Stack<Cube, FRACTAL_MAX_ITER> init_blocks;
void calcInitBlock(V3d pos) {
    init_blocks.clear();
    init_blocks.arr[0].vertex = { 0, 0, 0 };
    init_blocks.arr[0].a = 1.0; //整个活，如果你在栈空的时候依旧读取栈顶对应的 a，你会读出来 1，对应最大的那个块的边长
    if (pos.x <= 0 || pos.x >= 1 || pos.y <= 0 || pos.y >= 1 || pos.z <= 0 || pos.z >= 1)
        return;
    while (init_blocks.size() < FRACTAL_MAX_ITER) {
        Cube cube;
        V3d t(pos.x - init_blocks.top().vertex.x, pos.y - init_blocks.top().vertex.y, pos.z - init_blocks.top().vertex.z);
        cube.a = init_blocks.top().a / 3.0;
        cube.pos = { (int)(t.x / cube.a), (int)(t.y / cube.a), (int)(t.z / cube.a) };
        cube.vertex = (V3d){
            init_blocks.top().vertex.x + cube.pos.x * cube.a,
            init_blocks.top().vertex.y + cube.pos.y * cube.a,
            init_blocks.top().vertex.z + cube.pos.z * cube.a
        };
        init_blocks.push(cube);
        if (!inSponge(cube.pos))
            return;
    }
}

Ray res_ray;    // 待优化
V3d res_normal;
Stack<Cube, FRACTAL_MAX_ITER> blocks;
Stack<Cube, FRACTAL_MAX_ITER> temp_blocks;  //进行 calc 计算穿透一个方块进入下一个方块 之前 栈的状态。方便光追。

/// @brief 光线入射到面上之后，处理（栈中的）方块转换
/// @tparam CvtDouble space converter of double varibles
/// @tparam CvtInt space converter of int varibles
/// @param p 线面交点
/// @note 这么长其实是因为相似代码复制了一遍
template<typename CvtDouble, typename CvtInt>
void calc(CvtDouble p) {
    
    //每次在函数内判断 first_in 会损失一点性能，以后有时间管一下。
    const bool down = (p.X() == ((CvtDouble *)&(blocks.top().vertex))->X());   //是否是碰到了下表面

    if (down ^ blocks.empty()) {    //如果是第一次入射，那么碰到上表面是从上方射入新立方体；如果不是第一次入射，那么碰到旧立方体的下表面是从上方射入新立方体。
        
        if (!blocks.empty()) {
            while (!blocks.empty() && ((CvtInt *)&(blocks.top().pos))->X() == 0)   //出方块 【改】
                blocks.pop();
            if (blocks.empty())
                return;
            ((CvtInt *)&(blocks.top().pos))->X()--; //【改】
            ((CvtDouble *)&(blocks.top().vertex))->X() = ((CvtDouble *)&((blocks.cursor - 1)->vertex))->X() + blocks.top().a * ((CvtInt *)&(blocks.top().pos))->X();
            if (!inSponge(blocks.top().pos))
                return; //进入空方块：返回，进行下一次传播（propagate）。
        }
        while (blocks.size() < FRACTAL_MAX_ITER) {  //进方块
            const double y = p.Y() - ((CvtDouble *)&(blocks.top().vertex))->Y();
            const double z = p.Z() - ((CvtDouble *)&(blocks.top().vertex))->Z();
            Cube cube;
            cube.a = blocks.top().a / 3.0;  //整了个活，如果你在栈空的时候依旧读取栈顶对应的 a，你会读出来 1，对应最大的那个块的边长
            cube.pos = (CvtInt){ 2, (int)(y / cube.a), (int)(z / cube.a) }; //【改】
            cube.vertex = (V3d){
                blocks.top().vertex.x + cube.pos.x * cube.a,
                blocks.top().vertex.y + cube.pos.y * cube.a,
                blocks.top().vertex.z + cube.pos.z * cube.a
            };
            blocks.push(cube);
            if (!inSponge(cube.pos))
                break;
        }
    } else {
        if (!blocks.empty()) {
            while (!blocks.empty() && ((CvtInt *)&(blocks.top().pos))->X() == 2)   //出方块 【改】
                blocks.pop();
            if (blocks.empty())
                return;
            ((CvtInt *)&(blocks.top().pos))->X()++; //【改】
            ((CvtDouble *)&(blocks.top().vertex))->X() = ((CvtDouble *)&((blocks.cursor - 1)->vertex))->X() + blocks.top().a * ((CvtInt *)&(blocks.top().pos))->X();
            if (!inSponge(blocks.top().pos))
                return; //进入空方块：返回，进行下一次传播（propagate）。
        }
        while (blocks.size() < FRACTAL_MAX_ITER) {  //进方块
            const double y = p.Y() - ((CvtDouble *)&(blocks.top().vertex))->Y();
            const double z = p.Z() - ((CvtDouble *)&(blocks.top().vertex))->Z();
            Cube cube;
            cube.a = blocks.top().a / 3.0;  //整了个活，如果你在栈空的时候依旧读取栈顶对应的 a，你会读出来 1，对应最大的那个块的边长
            cube.pos = (CvtInt){ 0, (int)(y / cube.a), (int)(z / cube.a) }; //【改】
            cube.vertex = (V3d){
                blocks.top().vertex.x + cube.pos.x * cube.a,
                blocks.top().vertex.y + cube.pos.y * cube.a,
                blocks.top().vertex.z + cube.pos.z * cube.a
            };
            blocks.push(cube);
            if (!inSponge(cube.pos))
                break;
        }
    }
    
}

//单次光线计算
//三倍体代码（绷）
//return: 若撞到面了就返回 true
bool propagate(Ray ray) {
    while (true) {
        Cube cube = blocks.top();
        const bool x_hit = judgeHit(cube.vertex.x, cube.vertex.x + cube.a, ray.origin.x, ray.direction.x);
        const bool y_hit = judgeHit(cube.vertex.y, cube.vertex.y + cube.a, ray.origin.y, ray.direction.y);
        const bool z_hit = judgeHit(cube.vertex.z, cube.vertex.z + cube.a, ray.origin.z, ray.direction.z);
        const double x_surface = x_hit ? cube.vertex.x : cube.vertex.x + cube.a;
        const double y_surface = y_hit ? cube.vertex.y : cube.vertex.y + cube.a;
        const double z_surface = z_hit ? cube.vertex.z : cube.vertex.z + cube.a;
        std::pair<double, V3d> x_hit_info = ray.intersectSquare<dXYZ>((dXYZ){ x_surface, cube.vertex.y, cube.vertex.z }, cube.a);
        std::pair<double, V3d> y_hit_info = ray.intersectSquare<dYXZ>((dYXZ){ y_surface, cube.vertex.x, cube.vertex.z }, cube.a);
        std::pair<double, V3d> z_hit_info = ray.intersectSquare<dZXY>((dZXY){ z_surface, cube.vertex.x, cube.vertex.y }, cube.a);
        if (x_hit_info.first <= 0)  x_hit_info.first = INF; // 这三句在 judgeHit 里面好像判过了，可能可以删掉。
        if (y_hit_info.first <= 0)  y_hit_info.first = INF; // 这三句在 judgeHit 里面好像判过了，可能可以删掉。
        if (z_hit_info.first <= 0)  z_hit_info.first = INF; // 这三句在 judgeHit 里面好像判过了，可能可以删掉。
        const double temp_at = std::min({ x_hit_info.first, y_hit_info.first, z_hit_info.first });
        //if (cube.pos.x == 1 && cube.pos.y == 1 && cube.pos.z == 1)
        //    std::cout << "a";    //DEBUG
        //printf("%d %d %d\n", cube.pos.x, cube.pos.y, cube.pos.z);
        if (temp_at == INF) {
            // if (!blocks.empty()) //DEBUG
            //     res_normal = { 1, 1, 1 };
            // else
            //     res_normal = { 0.5, 0.5, 0.5 };
            res_ray.direction = ray.direction;
            return false;
        }

        temp_blocks = blocks;   // 非光追可删
        
        if      (x_hit_info.first == temp_at)   { ray.origin = x_hit_info.second; calc<dXYZ, XYZ<int> >(ray.origin); res_normal = { 1, 0, 0 }; }
        else if (y_hit_info.first == temp_at)   { ray.origin = y_hit_info.second; calc<dYXZ, YXZ<int> >(ray.origin); res_normal = { 0, 1, 0 }; }
        else if (z_hit_info.first == temp_at)   { ray.origin = z_hit_info.second; calc<dZXY, ZXY<int> >(ray.origin); res_normal = { 0, 0, 1 }; }
        if (blocks.size() >= FRACTAL_MAX_ITER) {
            res_ray = ray;
            return true;
        }
        else if (blocks.empty()) {
            res_ray.direction = ray.direction;
            return false;
        }
        //break;
    }
    return true;
}

/// @brief 光线追踪着色器（多次光线反射计算）
/// 现在暂时还是不带反射的版本
Color rayShader(Ray ray) {

    blocks = init_blocks;
    //memcpy(blocks.arr, init_blocks.arr, (init_blocks.size() + 2) * sizeof(Cube));   //这里好像 +1 就可以了，但是还是避免一下意外的溢出。
    //blocks.cursor = blocks.arr + init_blocks.size();

    bool hit = propagate(ray);
    //std::cout << hit << std::endl;
    if (!hit)   return SKY_COLOR;
    // else if (res_normal.x == 1)
    //     return { 1.0, 0.0, 0.0 };
    // else if (res_normal.y == 1)
    //     return { 0.0, 1.0, 0.0 };
    // //else if (res_normal.z == 1)
    //     return { 0.0, 0.0, 1.0 };
    return { res_normal.x, res_normal.y, res_normal.z };

}

// 计算距离趋向无穷时，此方向光线的亮度（假定光线在传播过程中不衰减）
Color calcLight(V3d direction) {
    return SKY_COLOR; //先不加方向光
}

// 生成 l~r 的随机数
double randLR(const double l, const double r) {
    return l + rng() / rng.max() * (r - l);
}

/// @brief 零到一之间的随机数
double rand01() {
    return rng() / (double)rng.max();
}

// 生成单位球体内的随机向量
V3d randBall() {
    const double r = rand01();
    const double theta = 2 * M_PI * rand01();   // 俯仰角
    const double phi = M_PI * rand01();         // 方位角
    const double sin_phi = sin(phi);
    return (V3d){ sin_phi * cos(theta), sin_phi * sin(theta), cos(phi) } * r;
}

// 适用于与坐标轴垂直的面，要求法向量标准化
V3d reflectVertical(V3d in, V3d normal) {
	if (abs(normal.x - 1) < DIST_EPS)
        return { -in.x, in.y, in.z };
    if (abs(normal.y - 1) < DIST_EPS)
        return { in.x, -in.y, in.z };
    // if (abs(normal.z) < DIST_EPS)
        return { in.x, in.y, -in.z };
}

// 光线偏转
void deflectRay(V3d &direction, V3d normal) {
    V3d res;
    direction.normalize();
    if (dot(direction, normal) < 0)
        normal = { -normal.x, -normal.y, -normal.z };   // 如果法向量是扎向面里的，那么把它反转
    while (true) {
        res = direction + randBall() * SPONGE_ROUGHNESS;
        if (dot(res, normal) > 0) {
            direction = res;
            return; //一直循环直到产生在面外的反射光线为止。
        }
    }
}

// 计算光线经过传播、反射等之后的颜色
Color RTShader(Ray ray) {
    blocks = init_blocks;
    res_ray = ray;
    Color color = { 2, 2, 2 };
    double reflect_param = 1;   // 俄罗斯轮盘赌的方法里 根据反射次数算出的这个光线的权值
    do {
        if (propagate(res_ray)) {   // 碰上了
            res_ray.direction = reflectVertical(res_ray.direction, res_normal); // 反射
            deflectRay(res_ray.direction, res_normal);
            blocks = temp_blocks;
            color *= (Color){ 0.8, 0.8, 0.8 };  // 这里定义海绵颜色
        } else {
            color *= calcLight(res_ray.direction);
            return color * reflect_param;
        }
        reflect_param *= 1 / REFLECT_PROBABILITY;
    } while (rand01() < REFLECT_PROBABILITY);
    return { 0, 0, 0 };
}

#endif
