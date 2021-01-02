#pragma once

#include <viewport/Viewport.hpp>
#include "entt/entt.hpp"

namespace prefabs
{
    struct ScoresOverlay
    {
        static void create(const std::shared_ptr<Viewport>& viewport);
    };
}
