#pragma once

#include "entt/entt.hpp"
#include "viewport/Viewport.hpp"

namespace prefabs
{
    struct CheatConsole
    {
        static entt::entity create(const std::shared_ptr<Viewport>& viewport);
    };
}
