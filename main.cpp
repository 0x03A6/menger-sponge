//渲染特定分形需要提供 fractal.hpp, fractal_settings.hpp，在 renderer.hpp 与 camera.hpp 中分别改为对应的名字即可。

#include <iostream>

#include "main_loop.hpp"

Renderer renderer;

int main() {
    //calcInitBlock({ 2, 2, 2 });
    //rayShader({ { 2, 2, 2 }, { -1.1, -1.2, -1.3 } });
    //rayShader({ { 2, 2, 2 }, { -1.1, -1.2, -1.3 } });
    mainLoopRT(renderer);
    //renderer.init();
    //renderer.render();
    getchar();
    return 0;
}