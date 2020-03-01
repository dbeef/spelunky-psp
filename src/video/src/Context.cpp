//
// Created by dbeef on 2/2/20.
//

#include "video/Context.hpp"
#include "Input.hpp"
#include "glad/glad.h"
#include "graphics_utils/DebugGlCall.hpp"

#include <SDL/SDL_video.h>
#include <SDL/SDL.h>
#include <cassert>

Video *Video::_instance = nullptr;

void Video::init()
{
    assert(!_instance);
    _instance = new Video();
}

void Video::dispose()
{
    assert(_instance);
    delete _instance;
    _instance = nullptr;
}

Video &Video::instance()
{
    assert(_instance);
    return *_instance;
}

void Video::tear_down_gl()
{
    SDL_Quit();
}

uint16_t Video::get_window_width()
{
    return 480;
}

uint16_t Video::get_window_height()
{
    return 272;
}

void Video::swap_buffers() const
{
    SDL_GL_SwapBuffers();
}

void Video::run_loop(std::function<void()> &loop_callback)
{

    auto& input = Input::instance();

    while (!input.isExit()) {

#ifndef NDEBUG
        DebugGlCall(glClear(GL_COLOR_BUFFER_BIT));
#endif

        input.poll();
        loop_callback();
        swap_buffers();

        // FIXME: Needs limiting FPS. Maybe stay with synchronizing to frame buffer swap?
    }
}
