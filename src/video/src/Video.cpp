//
// Created by dbeef on 2/2/20.
//

#include "video/Video.hpp"
#include "Input.hpp"
#include "SDL_opengl.h"
#include "graphics_utils/DebugGlCall.hpp"

void Video::loop_tick()
{
        auto& input = Input::instance();

        _timestep.mark_start();

#ifndef NDEBUG
        DebugGlCall(glClear(GL_COLOR_BUFFER_BIT));
#endif
         //exit = _loop_callback(_last_delta_ms);
        _loop_callback(_last_delta_ms);
        // Force GPU to render commands queued in the callback:
        DebugGlCall(glFlush());
        // Now CPU-consuming calls for the rest of the frame:
        // FIXME 2 callbacks? CPU callback + GPU callback?
        input.poll();

        swap_buffers();

        _timestep.mark_end();
        _last_delta_ms = _timestep.delay();
}

void Video::run_loop(const std::function<bool(uint32_t delta_time_ms)> &loop_callback)
{
    _loop_callback = loop_callback;
}
