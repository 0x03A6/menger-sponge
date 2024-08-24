#pragma once
#ifndef CAMREA_HPP
#define CAMERA_HPP

#include "vector.hpp"
#include "sponge.hpp"

struct Camera {

    V3d pos;
    V3d front, right, head;
    double v;   //移速

    Camera() {
        front = INIT_CAMERA_FRONT;
        head = INIT_CAMERA_HEAD;
        pos = INIT_CAMERA_POS;
        front.normalize();
        head.normalize();
        //pos.normalize();
        right = cross(front, head);
        v = INIT_CAMERA_VELOCITY;
    }
    
    //坐标系转换相关函数：
    V3d worldToCam(V3d a) const {
        return { dot(a, front), dot(a, right), dot(a, head) };
    }
    V3d camToWorld(V3d a) const {
        return worldToCam(a);
    }
    //将二维平面上的坐标转化为像素
    static Pixel screenToPixel(V2d point) {
        return { -(int)(point.y * SCALE) + H / 2, (int)(point.x * SCALE) + W / 2 };
    }
    //将像素转化为二维平面上的坐标
    static V2d pixelToScreen(Pixel pixel) {
        return { (double)(pixel.c - W / 2) / SCALE, (double)(-pixel.l + H / 2) / SCALE };
    }
    static V2d pixelToScreen(const double l, const double c) {
        return { (c - W / 2) / SCALE, (-l + H / 2) / SCALE };
    }
    V3d screenToSpace(V2d point) const {
        return pos + front * CAMERA_AXIS + right * point.x + head * point.y;
    }
    V3d pixelToSpace(Pixel px) const {
        return screenToSpace(pixelToScreen(px));
    }
    V3d pixelToSpace(const double l, const double c) const {
        return screenToSpace(pixelToScreen(l, c));
    }

    void moveLeft(const double delta) {
        pos -= right * delta * v;
    }
    void moveRight(const double delta) {
        pos += right * delta * v;
    }
    void moveUp(const double delta) {
        pos += head * delta * v;
    }
    void moveDown(const double delta) {
        pos -= head * delta * v;
    }
    void moveForward(const double delta) {
        pos += front * delta * v;
    }
    void moveBackward(const double delta) {
        pos -= front * delta * v;
    }
    //左右摇头，正数往左摇
    void rotateLR(const double delta) {
        right += front * delta;
        right.normalize();
        front = cross(head, right);
        //front.normalize();
    }
    //上下摇头，正数往上摇
    void rotateUD(const double delta) {
        front += head * delta * INIT_CAMERA_VELOCITY;
        front.normalize();
        head = cross(right, front);
        //head.normalize();
    }
};

#endif