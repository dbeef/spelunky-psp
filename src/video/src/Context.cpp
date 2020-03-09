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

// FIXME 2 callbacks? CPU callback + GPU callback?
void Video::run_loop(std::function<void()> &loop_callback)
{

    auto& input = Input::instance();

    while (!input.isExit()) {

#ifndef NDEBUG
        DebugGlCall(glClear(GL_COLOR_BUFFER_BIT));
#endif
        loop_callback();
        // Force GPU to render commands queued in the callback:
        DebugGlCall(glFlush());
        // Now CPU-consuming calls for the rest of the frame:
        input.poll();

        // FIXME: Needs limiting FPS. Maybe stay with synchronizing to frame buffer swap?
        swap_buffers();
    }
}

float Video::aspect()
{
    return static_cast<float>(Video::get_window_width()) / Video::get_window_height();
}
