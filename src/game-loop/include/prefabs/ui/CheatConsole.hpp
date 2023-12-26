#pragma once

#include "entt/entt.hpp"
#include "viewport/Viewport.hpp"
#include "game-loop/GameLoop.hpp"

#include <vector>
#include <functional>

namespace prefabs
{
    class CheatConsoleComponent
    {
    public:
        bool is_state_change_requested() const { return _state_change_requested; }
        void request_state_change(GameLoopState requested_state) { _requested_state = requested_state; _state_change_requested = true; }
        GameLoopState get_requested_state() const { return _requested_state;}
    private:
        bool _state_change_requested = false;
        GameLoopState _requested_state{GameLoopState::CURRENT};
    };

    struct CheatConsole
    {
        static entt::entity create(const std::shared_ptr<Viewport>& viewport);
    };
}
