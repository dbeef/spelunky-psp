#pragma once

#include <functional>

struct ImguiComponent
{
    std::function<void(int delta_time_ms)> render_callback;
};