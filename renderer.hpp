#pragma once
#ifndef RENDERER_HPP
#define RENDERER_HPP

#define SHOW_CONSOLE
#include <graphics.h>
#include <iostream>

#include "camera.hpp"
#include "scene.hpp"
#include "stack.hpp"

struct Renderer {
    Camera camera;
    Color graph[H][W];
    int count;

    void init() {
        initgraph(W, H, INIT_RENDERMANUAL);
        count = 0;
    }
    void render() {
        
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                graph[i][j] = rayShader({ camera.relative_pos, camera.pixelToSpace({ i, j }) - camera.relative_pos });
                putpixel(j, i, graph[i][j].toNum());
            }
        }
    };
    void renderRT() {
        count++;
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (count != 1)
                    graph[i][j] += RTShader({ camera.relative_pos, camera.pixelToSpace(i + rand01(), j + rand01()) - camera.relative_pos });
                else
                    graph[i][j] = RTShader({ camera.relative_pos, camera.pixelToSpace(i + rand01(), j + rand01()) - camera.relative_pos });  // 若是第一次累积，就清空之前的
                putpixel(j, i, (graph[i][j] / count).limited().toNum());
            }
        }
        std::cout << "accumulated frame count: " << count << '\n';
    }
    void renderAA() {
        count++;
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (count != 1)
                    graph[i][j] += rayShader({ camera.relative_pos, camera.pixelToSpace(i + rand01(), j + rand01()) - camera.relative_pos });
                else
                    graph[i][j] = rayShader({ camera.relative_pos, camera.pixelToSpace(i + rand01(), j + rand01()) - camera.relative_pos });  // 若是第一次累积，就清空之前的
#ifdef REAL_TIME_SHOW
                putpixel(j, i, (graph[i][j] / count).limited().toNum());
#endif
            }
        }
        std::cout << "count: " << count << '\n';
    }
    void clearAccumulate() {
        count = 0;
    }
};

#endif