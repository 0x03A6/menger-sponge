//渲染特定分形需要提供 fractal.hpp, fractal_settings.hpp，在 renderer.hpp 与 camera.hpp 中分别改为对应的名字即可。

#include <iostream>

#define REAL_TIME_SHOW

#include "main_loop.hpp"

Renderer renderer;

void recordPath() {
    mainLoopPR(renderer);
    path_recorder.smoothenPath();
    path_recorder.storgePath("PATH");
    path_recorder.showPath(renderer);
}

void generateImages() {
    path_recorder.readPath("PATH");
    renderer.init();
    path_recorder.showPath(renderer);
    path_recorder.generateImage(renderer);
}

int main() {
    //calcInitBlock({ 2, 2, 2 });
    //rayShader({ { 2, 2, 2 }, { -1.1, -1.2, -1.3 } });
    //rayShader({ { 2, 2, 2 }, { -1.1, -1.2, -1.3 } });
    //renderer.init();
    //renderer.render();
    mainLoopAA(renderer);

    //recordPath();
    //generateImages();
    
    getchar();
    return 0;
}