#pragma once
#ifndef MAIN_LOOP_HPP
#define MAIN_LOOP_HPP

#include <iostream>

#include "renderer.hpp"
#include "path_recorder.hpp"
#include "image_output.hpp"

inline void mainLoop(Renderer &renderer) {
    renderer.init();
    calcInitBlock(renderer.camera.pos);
    renderer.camera.relative_pos = enterInitBlock(renderer.camera.pos);
    renderer.render();

    double move_delta = 0.1;
    setbkcolor(WHITE);

    //鼠标放在屏幕中心位置
    const int x_cursor_pos = GetSystemMetrics(SM_CXFULLSCREEN) / 2;
    const int y_cursor_pos = GetSystemMetrics(SM_CYFULLSCREEN) / 2;
    SetCursorPos(x_cursor_pos, y_cursor_pos);
    bool alt_pressed = true;   //alt在上次检测的时候是否被按下
    while (true) {
        bool moved = false;
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)  move_delta = 0.3;   //葱刺！！！
        else                                        move_delta = 0.1;
        if (GetAsyncKeyState('W') & 0x8000) renderer.camera.moveForward(move_delta), moved = true;
        if (GetAsyncKeyState('A') & 0x8000) renderer.camera.moveLeft(move_delta), moved = true;
        if (GetAsyncKeyState('S') & 0x8000) renderer.camera.moveBackward(move_delta), moved = true;
        if (GetAsyncKeyState('D') & 0x8000) renderer.camera.moveRight(move_delta), moved = true;
        if (GetAsyncKeyState(VK_SPACE) & 0x8000)    renderer.camera.moveUp(move_delta), moved = true;
        if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) renderer.camera.moveDown(move_delta), moved = true;
        if (GetAsyncKeyState(VK_MENU) & 0x8000) alt_pressed = true;
        else {
            if (alt_pressed) {
                //如果alt刚被松开，那么只把鼠标移至中心，而不计算旋转
                alt_pressed = false;
                SetCursorPos(x_cursor_pos, y_cursor_pos);
            } else {
                POINT delta_pos;
                GetCursorPos(&delta_pos);
                delta_pos.x = x_cursor_pos - delta_pos.x;
                delta_pos.y = y_cursor_pos - delta_pos.y;
                if (delta_pos.x != 0 || delta_pos.y != 0) {
                    renderer.camera.rotateUD(1.0 * delta_pos.y / H);
                    renderer.camera.rotateLR(1.0 * delta_pos.x / W);
                    moved = true;
                    SetCursorPos(x_cursor_pos, y_cursor_pos);
                }
            }
        }
        if (moved) {
            //cleardevice();
            calcInitBlock(renderer.camera.pos);
            renderer.camera.relative_pos = enterInitBlock(renderer.camera.pos);
            renderer.render();
        }
        //Sleep(10);
        delay_ms(0);
    }
}

// 记录路径
inline void mainLoopPR(Renderer &renderer) {
    renderer.init();
    calcInitBlock(renderer.camera.pos);
    renderer.camera.relative_pos = enterInitBlock(renderer.camera.pos);
    renderer.render();

    double move_delta = 0.1;
    setbkcolor(WHITE);

    //鼠标放在屏幕中心位置
    const int x_cursor_pos = GetSystemMetrics(SM_CXFULLSCREEN) / 2;
    const int y_cursor_pos = GetSystemMetrics(SM_CYFULLSCREEN) / 2;
    SetCursorPos(x_cursor_pos, y_cursor_pos);
    bool alt_pressed = true;   //alt在上次检测的时候是否被按下
    path_recorder.beginRecord();
    while (true) {
        bool moved = false;
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)  move_delta = 0.3;   //葱刺！！！
        else                                        move_delta = 0.1;
        if (GetAsyncKeyState('W') & 0x8000) renderer.camera.moveForward(move_delta), moved = true;
        if (GetAsyncKeyState('A') & 0x8000) renderer.camera.moveLeft(move_delta), moved = true;
        if (GetAsyncKeyState('S') & 0x8000) renderer.camera.moveBackward(move_delta), moved = true;
        if (GetAsyncKeyState('D') & 0x8000) renderer.camera.moveRight(move_delta), moved = true;
        if (GetAsyncKeyState(VK_SPACE) & 0x8000)    renderer.camera.moveUp(move_delta), moved = true;
        if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) renderer.camera.moveDown(move_delta), moved = true;
        if (GetAsyncKeyState(VK_MENU) & 0x8000) alt_pressed = true;
        else {
            if (alt_pressed) {
                //如果alt刚被松开，那么只把鼠标移至中心，而不计算旋转
                alt_pressed = false;
                SetCursorPos(x_cursor_pos, y_cursor_pos);
            } else {
                POINT delta_pos;
                GetCursorPos(&delta_pos);
                delta_pos.x = x_cursor_pos - delta_pos.x;
                delta_pos.y = y_cursor_pos - delta_pos.y;
                if (delta_pos.x != 0 || delta_pos.y != 0) {
                    renderer.camera.rotateUD(1.0 * delta_pos.y / H);
                    renderer.camera.rotateLR(1.0 * delta_pos.x / W);
                    moved = true;
                    SetCursorPos(x_cursor_pos, y_cursor_pos);
                }
            }
        }
        if (GetAsyncKeyState('L') & 0x8000) {
            path_recorder.calcPath();
            return;
        }
        path_recorder.addPoint(renderer.camera);
        if (moved) {
            //cleardevice();
            calcInitBlock(renderer.camera.pos);
            renderer.camera.relative_pos = enterInitBlock(renderer.camera.pos);
            renderer.render();
        }
        //Sleep(10);
        delay_ms(0);
    }
}

inline void mainLoopRT(Renderer &renderer) {
    renderer.init();
    calcInitBlock(renderer.camera.pos);
    renderer.camera.relative_pos = enterInitBlock(renderer.camera.pos);
    renderer.render();

    double move_delta = 0.1;
    setbkcolor(WHITE);

    //鼠标放在屏幕中心位置
    const int x_cursor_pos = GetSystemMetrics(SM_CXFULLSCREEN) / 2;
    const int y_cursor_pos = GetSystemMetrics(SM_CYFULLSCREEN) / 2;
    SetCursorPos(x_cursor_pos, y_cursor_pos);
    bool alt_pressed = true;   //alt在上次检测的时候是否被按下
    bool l_pressed = false;
    bool locked = false;

    while (true) {

        if (GetAsyncKeyState('L') & 0x8000) {
            if (!l_pressed) {
                
                locked = !locked;
                l_pressed = true;
                if (locked) {
                    std::cout << "locked\n";
                } else {
                    alt_pressed = true; // 每次解锁的时候忽略最开始的鼠标位置造成的移动
                    std::cout << "unlocked\n";
                }
            }
        } else
            l_pressed = false;
        if (locked) {
            renderer.renderRT();
            delay_ms(0);
            continue;
        }

        bool moved = false;
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)  move_delta = 0.3;   //葱刺！！！
        else                                        move_delta = 0.1;
        if (GetAsyncKeyState('W') & 0x8000) renderer.camera.moveForward(move_delta), moved = true;
        if (GetAsyncKeyState('A') & 0x8000) renderer.camera.moveLeft(move_delta), moved = true;
        if (GetAsyncKeyState('S') & 0x8000) renderer.camera.moveBackward(move_delta), moved = true;
        if (GetAsyncKeyState('D') & 0x8000) renderer.camera.moveRight(move_delta), moved = true;
        if (GetAsyncKeyState(VK_SPACE) & 0x8000)    renderer.camera.moveUp(move_delta), moved = true;
        if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) renderer.camera.moveDown(move_delta), moved = true;
        if (GetAsyncKeyState(VK_MENU) & 0x8000) alt_pressed = true;
        else {
            if (alt_pressed) {
                //如果alt刚被松开，那么只把鼠标移至中心，而不计算旋转
                alt_pressed = false;
                SetCursorPos(x_cursor_pos, y_cursor_pos);
            } else {
                POINT delta_pos;
                GetCursorPos(&delta_pos);
                delta_pos.x = x_cursor_pos - delta_pos.x;
                delta_pos.y = y_cursor_pos - delta_pos.y;
                if (delta_pos.x != 0 || delta_pos.y != 0) {
                    renderer.camera.rotateUD(1.0 * delta_pos.y / H);
                    renderer.camera.rotateLR(1.0 * delta_pos.x / W);
                    moved = true;
                    SetCursorPos(x_cursor_pos, y_cursor_pos);
                }
            }
        }
        if (moved) {
            //cleardevice();
            calcInitBlock(renderer.camera.pos);
            renderer.camera.relative_pos = enterInitBlock(renderer.camera.pos);
            renderer.render();
            renderer.clearAccumulate();
        } else {
            renderer.renderRT();
        }
        //Sleep(10);
        delay_ms(0);
    }
}

// 抗锯齿
inline void mainLoopAA(Renderer &renderer) {
    renderer.init();
    calcInitBlock(renderer.camera.pos);
    renderer.camera.relative_pos = enterInitBlock(renderer.camera.pos);
    renderer.render();

    double move_delta = 0.1;
    setbkcolor(WHITE);

    //鼠标放在屏幕中心位置
    const int x_cursor_pos = GetSystemMetrics(SM_CXFULLSCREEN) / 2;
    const int y_cursor_pos = GetSystemMetrics(SM_CYFULLSCREEN) / 2;
    SetCursorPos(x_cursor_pos, y_cursor_pos);
    bool alt_pressed = true;   //alt在上次检测的时候是否被按下
    bool l_pressed = false;
    bool locked = false;

    while (true) {

        if (GetAsyncKeyState('L') & 0x8000) {
            if (!l_pressed) {
                
                locked = !locked;
                l_pressed = true;
                if (locked) {
                    std::cout << "locked\n";
                } else {
                    alt_pressed = true; // 每次解锁的时候忽略最开始的鼠标位置造成的移动
                    std::cout << "unlocked\n";
                }
            }
        } else
            l_pressed = false;
        if (locked) {
            renderer.renderAA();
            delay_ms(0);
            continue;
        }

        bool moved = false;
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)  move_delta = 0.3;   //葱刺！！！
        else                                        move_delta = 0.1;
        if (GetAsyncKeyState('W') & 0x8000) renderer.camera.moveForward(move_delta), moved = true;
        if (GetAsyncKeyState('A') & 0x8000) renderer.camera.moveLeft(move_delta), moved = true;
        if (GetAsyncKeyState('S') & 0x8000) renderer.camera.moveBackward(move_delta), moved = true;
        if (GetAsyncKeyState('D') & 0x8000) renderer.camera.moveRight(move_delta), moved = true;
        if (GetAsyncKeyState(VK_SPACE) & 0x8000)    renderer.camera.moveUp(move_delta), moved = true;
        if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) renderer.camera.moveDown(move_delta), moved = true;
        if (GetAsyncKeyState(VK_MENU) & 0x8000) alt_pressed = true;
        else {
            if (alt_pressed) {
                //如果alt刚被松开，那么只把鼠标移至中心，而不计算旋转
                alt_pressed = false;
                SetCursorPos(x_cursor_pos, y_cursor_pos);
            } else {
                POINT delta_pos;
                GetCursorPos(&delta_pos);
                delta_pos.x = x_cursor_pos - delta_pos.x;
                delta_pos.y = y_cursor_pos - delta_pos.y;
                if (delta_pos.x != 0 || delta_pos.y != 0) {
                    renderer.camera.rotateUD(1.0 * delta_pos.y / H);
                    renderer.camera.rotateLR(1.0 * delta_pos.x / W);
                    moved = true;
                    SetCursorPos(x_cursor_pos, y_cursor_pos);
                }
            }
        }
        if (moved) {
            //cleardevice();
            renderer.clearAccumulate();
            calcInitBlock(renderer.camera.pos);
            renderer.camera.relative_pos = enterInitBlock(renderer.camera.pos);
            renderer.renderAA();
        } else {
            renderer.renderAA();
        }
        //Sleep(10);
        delay_ms(0);
    }
}

#endif