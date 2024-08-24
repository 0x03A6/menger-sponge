## 介绍

- 用 C++ 和 EGE 实现的渲染了门格海绵的简单的软渲染器。可以使用光线追踪（和无反射的光线追踪）进行渲染。
- 光线追踪算法还比较简陋，但是渲染结果还能看。打算以后用 BRDF 什么的重新写一遍。（但是最近上高三作业太多了，没啥时间。）
- 演示视频（无反射光线追踪版本）：https://www.bilibili.com/video/BV19qp9eoERq

## 功能

- 渲染门格海绵的图像并显示在屏幕上。
- 可以使用 `WASD` 控制摄像机前后左右移动，`SPACE` 控制摄像机上移，左 `CTRL` 控制摄像机下移；按住鼠标右键加速移动；在光追渲染中，按下 `L` 键可以给操作加锁/解锁（可能得稍微多按会儿），锁定状态下除了 `L` 键以外所有操作无效。
- 光追渲染中，为了保证移动时不会太卡顿，每次移动后的第一次渲染是无反射的光追渲染，第二次开始才是正常光追。
- 光追渲染中，若两帧间未做任何移动，那么会显示多帧合成的图像。

## 编译

- 可行的编译命令：`g++ main.cpp -o main.exe -O2 -lgraphics64 -luuid -lmsimg32 -lgdi32 -limm32 -lole32 -loleaut32 -lwinmm -lgdiplus`

## Introduction

- A simple soft renderer written with C++ and EGE. It can render a Menger Sponge with Ray Tracing(and Ray Tracing without reflecting).
- The Ray Tracing algorithm is relatively poor, but the rendering results is still not too bad. I plan to rewrite it using BRDF or something in the future. (But recently there have been too many homework. I don't have much time.)
- Demo video(non-reflective ray tracing version): https://www.bilibili.com/video/BV19qp9eoERq

## Functions
- Render images of Menger Sponge and display them on screen.
- You can move the camera forward, backward, up and down by pressing `W`, `A`, `S` and `D`. Pressing `SPACE` can make it move up and pressing left `CTRL` can make it move down. In Ray Tracing rendering, you can press `L` to lock/unlock your operation(maybe you should press the key a bit longer). When operations are locked, all of the keys are invalid except `L`.
- In Ray Tracing rendering, in order to ensure that the movement will not get too laggy, the first rendering after each movement is non reflective Ray Tracing rendering, and the second is normal ray tracing.
- In ray tracing rendering, if there is no movement between two frames, a composite image of multiple frames will be displayed.

## ‌Compilation

- An avilable compiling command: `g++ main.cpp -o main.exe -O2 -lgraphics64 -luuid -lmsimg32 -lgdi32 -limm32 -lole32 -loleaut32 -lwinmm -lgdiplus`
