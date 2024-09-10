#pragma once

#ifndef IMAGE_OUTPUT_HPP
#define IMAGE_OUTPUT_HPP

#include <iostream>
#include <cstring>
#include <algorithm>

#include "sponge.hpp"

inline char *toStr(int x) {
    static char temp[20] = "";
    memset(temp, 0, 20);
    int i = -1;
    if (x == 0) {
        i = 0;
        temp[0] = '0';
    } else {
        for (; x != 0; ) {
            i++;
            temp[i] = (x % 10) | '0';
            x /= 10;
        }
        for (int j = 0; j <= (i >> 1); j++) {
            std::swap(temp[i - j], temp[j]);
        }
    }
    temp[i + 1] = '.';
    temp[i + 2] = 'p';
    temp[i + 3] = 'p';
    temp[i + 4] = 'm';
    return temp;
}

FILE *fp;

void pixel_write(Color _color) {
    static unsigned char color[3];
    color[0] = (int)(_color.r * 255.999);
    color[1] = (int)(_color.g * 255.999);
    color[2] = (int)(_color.b * 255.999);
    fwrite(color, 1, 3, fp);
}


void toImage(char *name, Color *px_map) {
    fp = fopen(name, "wb");
    fprintf(fp, "P6\n%d %d\n255\n", W, H);
    for(int j = 0; j < W; j++)
        for(int i = 0; i < H; i++) {
            pixel_write(px_map[j * H + i]);
        }
    fclose(fp);
    printf(name);
    putchar('\n');
}

#endif