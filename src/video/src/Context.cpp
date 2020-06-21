//
// Created by dbeef on 2/2/20.
//

#include "video/Context.hpp"
#include "Input.hpp"
#include "glad/glad.h"
#include "graphics_utils/DebugGlCall.hpp"
#include "time/Timestep.hpp"

#include <SDL_opengl.h>
#include <SDL_video.h>
#include <SDL.h>


void Video::tear_down_gl()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Video::swap_buffers() const
{
    SDL_GL_SwapWindow(window);
}

void Video::run_loop(const std::function<void(uint32_t delta_time_ms)> &loop_callback)
{

    auto& input = Input::instance();

    while (!input.isExit()) {

        _timestep.mark_start();

#ifndef NDEBUG
        DebugGlCall(glClear(GL_COLOR_BUFFER_BIT));
#endif
        loop_callback(_last_delta_time);
        // Force GPU to render commands queued in the callback:
        DebugGlCall(glFlush());
        // Now CPU-consuming calls for the rest of the frame:
        // FIXME 2 callbacks? CPU callback + GPU callback?
        input.poll();

        swap_buffers();

        _timestep.mark_end();
        _last_delta_time = _timestep.delay();
    }
}
