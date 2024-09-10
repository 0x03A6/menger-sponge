#pragma once

#ifndef PATH_RECORDER_HPP
#define PATH_RECORDER_HPP

#include <iostream>
#include <vector>
#include <graphics.h>

#include "image_output.hpp"
#include "renderer.hpp"

struct PathRecorder {
    struct PointInfo {
        double t;   // 时间
        CamInfo cam;
    };
    
    std::vector<PointInfo> original_path;
    std::vector<CamInfo> path;
    CamInfo temp_path[N];
    double t_begin;
    void beginRecord() {
        t_begin = fclock();
    }
    void addPoint(CamInfo cam) {
        original_path.push_back({ fclock() - t_begin, cam });
    }

    // 修正相机的上方、右侧向量，保证它们与前方向量互相垂直
    void correctCamInfo(CamInfo &info) {
        info.front.normalize();
        info.right -= info.front * dot(info.right, info.front);
        info.right.normalize();
        info.head = cross(info.right, info.front);
    }

    // 整理出 25 帧的路径
    void calcPath() {
        const double t_total = original_path.back().t;
        original_path.push_back({ INFINITY, {}}); // 放一个无穷在最后，可以让底下的（内层）循环少一个判断
        std::vector<PointInfo>::iterator it = original_path.begin();
        for (double i = original_path.begin()->t; i < t_total; i += 1.0 / 25.0) {
            for (; it->t <= i; it++);
            it--;
            const double delta_t = (it + 1)->t - it->t;
            const double v1 = (((it + 1)->t - i) / delta_t);    //v1 & v2: 两边的权值
            const double v2 = ((i - it->t) / delta_t);
            path.push_back({
                it->cam.pos * v1 + (it + 1)->cam.pos * v2,
                it->cam.front * v1 + (it + 1)->cam.front * v2,
                it->cam.right * v1 + (it + 1)->cam.right * v2,
                { 0, 0, 0 } // head 会在 correctCamInfo 里面生成，这里不需要
            });
            correctCamInfo(path.back());
            //printf("%lf %lf %lf\n", path.back().pos.x, path.back().pos.y, path.back().pos.z);   // DEBUG
        }
    }

    void smoothenPath() {
        for (int i = 0; i < 20; i++) {
            temp_path[0] = path[0];
            for (int j = 1; j < path.size() - 1; j++) {
                temp_path[j].front = path[j].front * 1 + path[j - 1].front * 1 + path[j + 1].front * 1;
                temp_path[j].right = path[j].right * 1 + path[j - 1].right * 1 + path[j + 1].right * 1;
                correctCamInfo(temp_path[j]);
                temp_path[j].pos = path[j].pos * 0.8 + path[j - 1].pos * 0.1 + path[j + 1].pos * 0.1;
            }
            for (int j = 1; j < path.size() - 1; j++) {
                path[j] = temp_path[j];
            }
            //memcpy(path.data(), temp_path, path.size() * sizeof(CamInfo));    //BUG
        }
    }

    void generateImage(Renderer &renderer) {
        for (int i = 0; i < path.size(); i++) {
            renderer.camera.pos = path[i].pos;
            renderer.camera.head = path[i].head;
            renderer.camera.front = path[i].front;
            renderer.camera.right = path[i].right;
            calcInitBlock(renderer.camera.pos);
            renderer.camera.relative_pos = enterInitBlock(renderer.camera.pos);
            renderer.clearAccumulate();
            for (int j = 0; j < 50; j++) {
                renderer.renderAA();
#ifdef REAL_TIME_SHOW
                delay_ms(0);
#endif
            }
            for (int j = 0; j < H; j++)
                for (int k = 0; k < W; k++)
                    renderer.graph[j][k] /= (double)renderer.count;
            toImage(toStr(i), (Color *)renderer.graph);
        }
    }
    void showPath(Renderer &renderer) {
        for (int i = 0; i < path.size(); i++) {
            renderer.camera.pos = path[i].pos;
            renderer.camera.head = path[i].head;
            renderer.camera.front = path[i].front;
            renderer.camera.right = path[i].right;
            calcInitBlock(renderer.camera.pos);
            renderer.camera.relative_pos = enterInitBlock(renderer.camera.pos);
            renderer.render();
            delay_ms(0);
        }
    }
    void storgePath(char *name) {
        FILE *fp = fopen(name, "wb");
        int l = path.size();
        fwrite(&l, 1, 4, fp);
        for (int i = 0; i < path.size(); i++)
            fwrite(&path[i], 1, sizeof(CamInfo), fp);
    }
    void readPath(char *name) {
        FILE *fp = fopen(name, "rb");
        path.clear();
        int l;
        fread(&l, 1, 4, fp);
        for (int i = 0; i < l; i++) {
            CamInfo temp;
            fread(&temp, 1, sizeof(CamInfo), fp);
            path.push_back(temp);
        }
    }
} path_recorder;

#endif