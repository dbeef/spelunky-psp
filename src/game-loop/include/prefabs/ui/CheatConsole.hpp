#pragma once

#include "entt/entt.hpp"
#include "viewport/Viewport.hpp"
#include "game-loop/GameLoop.hpp"

namespace prefabs
{
    struct CheatConsoleComponent
    {
        GameLoop::State state{GameLoop::State::CURRENT};
    };

    struct CheatConsole
    {
        static entt::entity create(const std::shared_ptr<Viewport>& viewport);
    };
}
