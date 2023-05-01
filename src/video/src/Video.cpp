//
// Created by dbeef on 2/2/20.
//

#include "video/Video.hpp"
#include "Input.hpp"
#include "glad/glad.h"
#include "graphics_utils/DebugGlCall.hpp"

void Video::run_loop(const std::function<bool(const FrameStats& frame_stats)> &loop_callback)
{
    auto& input = Input::instance();

    FrameStats frame_stats{};
    bool exit = false;

    while (!exit)
    {

        _timestep.mark_start();

#ifndef NDEBUG
        DebugGlCall(glClear(GL_COLOR_BUFFER_BIT));
#endif
         exit = loop_callback(frame_stats);
        // Force GPU to render commands queued in the callback:
        DebugGlCall(glFlush());
        // Now CPU-consuming calls for the rest of the frame:
        // FIXME 2 callbacks? CPU callback + GPU callback?
        input.poll();

        _timestep.mark_end();
        frame_stats.game_loop_update_time_ms =  _timestep.get_delta_time_ms();

        swap_buffers();

        _timestep.mark_end();
        frame_stats.total_delta_time_ms = _timestep.delay();

        if (exit)
        {
            break;
        }
    }
}
