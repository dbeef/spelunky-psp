// TODO: Disable adding this signature in IDE. Add a line in coding guideline concerning this.
//
// Created by dbeef on 7/7/19.
//

#include <SDL/SDL.h>
#include <iostream>
#include <Input.hpp>

#include "Context.hpp"
#include "glad/glad.h"

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
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        SDL_ClearError();
        return false;
    }

    //  Create a window
    auto surface = SDL_SetVideoMode(getWindowWidth(),
                                    getWindowHeight(),
                                    0, // current display's bpp
                                    SDL_DOUBLEBUF | SDL_OPENGL | SDL_SWSURFACE);

    if (!surface) {
        std::cout << "SDL_SetVideoMode Error: " << SDL_GetError() << std::endl;
        SDL_ClearError();
        return false;
    }

    if(!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        printf("Error while loading ptrs to OpenGL functions\n");
        return false;
    }

    // TODO: OpenGL asserts.
    glShadeModel(GL_SMOOTH);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void Video::tearDownGL() {
    SDL_Quit();
}

// FIXME: Duplicating camera's consts
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
    SDL_Event event;
    bool running = true;

    while (running) {

        while (SDL_PollEvent(&event)) {
            running = event.type != SDL_QUIT;
        }
        input.process(event);

        loopCallback();

        swapBuffers();
    }
}

