//
// Created by dbeef on 2/2/20.
//

#include <video/Context.hpp>
#include <SDL/SDL_video.h>
#include <SDL/SDL.h>
#include <time/Timestep.hpp>
#include <Input.hpp>
#include <cassert>
#include <glad/glad.h>
#include <graphics_utils/DebugGlCall.hpp>

Video *Video::_instance = nullptr;

void Video::init() {
    assert(!_instance);
    _instance = new Video();
}

void Video::dispose() {
    assert(_instance);
    delete _instance;
    _instance = nullptr;
}

Video &Video::instance() {
    assert(_instance);
    return *_instance;
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

void Video::swapBuffers() const {
    SDL_GL_SwapBuffers();
}

void Video::runLoop(std::function<void()> &loopCallback) {

    auto& input = Input::instance();

    Timestep t(60);

    while (!input.isExit()) {


        t.mark_start();

        input.poll();
        loopCallback();
        swapBuffers();

        t.mark_end();
        t.delay();
    }
}
