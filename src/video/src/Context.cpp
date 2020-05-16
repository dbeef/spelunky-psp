//
// Created by dbeef on 2/2/20.
//

#include "video/Context.hpp"
#include "Input.hpp"
#include "glad/glad.h"
#include "graphics_utils/DebugGlCall.hpp"
#include "time/Timestep.hpp"

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

uint16_t Video::get_window_width() const
{
    return _width;
}

uint16_t Video::get_window_height() const
{
    return _height;
}

void Video::swap_buffers() const
{
    SDL_GL_SwapBuffers();
}

void Video::run_loop(const std::function<void(uint32_t delta_time_ms)> &loop_callback)
{

    auto& input = Input::instance();

    static uint32_t last_delta_ms = 0;

    while (!input.isExit()) {

        _timestep.mark_start();

#ifndef NDEBUG
        DebugGlCall(glClear(GL_COLOR_BUFFER_BIT));
#endif
        loop_callback(last_delta_ms);
        // Force GPU to render commands queued in the callback:
        DebugGlCall(glFlush());
        // Now CPU-consuming calls for the rest of the frame:
        // FIXME 2 callbacks? CPU callback + GPU callback?
        input.poll();

        swap_buffers();

        _timestep.mark_end();
        _timestep.delay();
        last_delta_ms = _timestep.get_delta_ms();
    }
}

float Video::get_aspect() const
{
    return _aspect;
}
