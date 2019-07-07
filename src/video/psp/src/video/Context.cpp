// TODO: Disable adding this signature in IDE. Add a line in coding guideline concerning this.
//
// Created by dbeef on 7/7/19.
//

#include <SDL/SDL.h>
#include <pspdebug.h>

#include "Context.hpp"

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

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // FIXME: "No available video device" on PSP!
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
    SDL_Event event;
    bool running = true;

    while (running) {

        while (SDL_PollEvent(&event)) {
            running = event.type != SDL_QUIT;
        }

        loopCallback();

        swapBuffers();
    }
}

