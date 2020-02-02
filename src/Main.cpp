#include "video/Context.hpp"
#include "video/GL.hpp"
#include "logger/log.h"
#include "Camera.hpp"
#include "LevelGenerator.hpp"
#include "LevelRenderer.hpp"

#include <cmath>
#include <cstdlib>

int start() {
    log_info("Started.");

    Camera::init();
    LevelGenerator::init();
    LevelRenderer::init();

    Video::init();

    if(!Video::instance().setupGL())
    {
        log_error("Failed to setup OpenGL.");
        return EXIT_FAILURE;
    }

    std::function<void()> callback = []() {
        static float r = 0;
        r += 0.01f;
        glClearColor(std::sin(r), 0.4f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    };

    Video::instance().runLoop(callback);
    Video::instance().tearDownGL();
    Video::dispose();

    Camera::dispose();
    LevelGenerator::dispose();
    LevelRenderer::dispose();

    log_info("Exiting peacefully.");
    return EXIT_SUCCESS;
}

#if defined(SPELUNKY_PSP_PLATFORM_PSP)
// Not mangling symbols so SDL could find SDL_main.
extern "C"
{
int SDL_main(int argc, char *argv[]) {
    return start();
}
}
#endif

#if defined(SPELUNKY_PSP_PLATFORM_LINUX)
int main() {
    return start();
}
#endif
