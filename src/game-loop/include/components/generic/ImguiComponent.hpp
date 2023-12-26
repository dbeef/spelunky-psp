#pragma once

#include <functional>

struct ImguiComponent
{
    std::function<void()> render_callback;
};