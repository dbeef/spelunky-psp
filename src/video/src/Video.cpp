//
// Created by dbeef on 2/2/20.
//

#include "video/Video.hpp"
#include "Input.hpp"
#include "glad/glad.h"
#include "graphics_utils/DebugGlCall.hpp"

REGISTER_SINGLETON_INSTANCE(Video)

bool Video::tick(const TickCallback& tick_callback)
{
        auto& input = Input::instance();

        _timestep.mark_start();

#ifndef NDEBUG
        DebugGlCall(glClear(GL_COLOR_BUFFER_BIT));
#endif
        auto result = tick_callback(_last_delta_ms);
        // Force GPU to render commands queued in the callback:
        DebugGlCall(glFlush());
        // Now CPU-consuming calls for the rest of the frame:
        // FIXME 2 callbacks? CPU callback + GPU callback?
        input.poll();

        swap_buffers();

        _timestep.mark_end();
        _last_delta_ms = _timestep.delay();

        return result;
}
