// TODO: Disable adding this signature in IDE. Add a line in coding guideline concerning this.
//
// Created by dbeef on 7/7/19.
//

#include <SDL/SDL.h>
#include <pspdebug.h>
#include <cassert>

#include <Input.hpp>
#include "Context.hpp"
#include "video/GL.hpp"

#define printf pspDebugScreenPrintf

Video *Video::_instance = nullptr;

void Video::init() {
    // TODO: Assertions
    _instance = new Video();
}

void Video::dispose() {
    // TODO: Assertions
    delete _instance;
    _instance = nullptr;
}

Video &Video::instance() {
    // TODO: Assertions
    return *_instance;
}

bool Video::setupGL() {



    printf("Entered Video::setupGL\n");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    //  Create a window
    auto surface = SDL_SetVideoMode(getWindowWidth(),
                                    getWindowHeight(),
                                    0, // current display's bpp
                                    SDL_DOUBLEBUF | SDL_OPENGL | SDL_SWSURFACE);

    if (!surface) {
        printf("SDL_SetVideoMode Error: %s\n", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    if(!gladLoadGLES1Loader((GLADloadproc) SDL_GL_GetProcAddress)) {
        printf("Error while loading ptrs to OpenGL functions\n");
        return false;
    }

    SDL_GL_LoadLibrary(nullptr);
    SDL_ClearError();

    // TODO: OpenGL asserts.
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    printf("Exiting Video::setupGL, success.\n");
    return true;
}

void Video::tearDownGL() {
    SDL_Quit();
}

uint16_t Video::getWindowWidth() {
    return 480;
}

uint16_t Video::getWindowHeight() {
    return 272;
}

void Video::swapBuffers() {
    SDL_GL_SwapBuffers();
}

void Video::runLoop(std::function<void()> &loopCallback) {

    auto& input = Input::instance();

    while (!input.isExit()) {
        input.poll();
        loopCallback();
        swapBuffers();
    }
}

