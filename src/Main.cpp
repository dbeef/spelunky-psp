#include "video/Context.hpp"
#include "video/GL.hpp"
#include <pspdebug.h>
#include <cmath>
#include <SDL/SDL_video.h>
#include "GL/glut.h"
#include "GLES/egl.h"

#define printf pspDebugScreenPrintf

int start() {
    Video::init();
    if(!Video::instance().setupGL()) return 1;

    static bool been_there = false;

    std::function<void()> callback = []() {
        static float r = 0;
        r += 0.01f;

        if(!eglGetProcAddress("glClearColor") || !eglGetProcAddress("glClear"))
        {
            if(!been_there)
            {
                auto err = SDL_GetError();

                printf("\nError: %s\n", err);
                printf("\nError: %s\n", err);
                printf("\nError: %s\n", err);
                printf("\nError: %s\n", err);
                printf("\nError: %s\n", err);
                been_there = true;
            }
            return;
        }

        glClearColor(std::sin(r), 0.4f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    };

    Video::instance().runLoop(callback);
    Video::instance().tearDownGL();
    Video::dispose();

    return 0;
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
