#pragma once

#include "entt/entt.hpp"
#include "viewport/Viewport.hpp"
#include "game-loop/GameLoop.hpp"

namespace prefabs
{
    class CheatConsoleComponent
    {
    public:
        bool is_state_change_requested() const { return _state_change_requested; }
        void request_state_change(GameLoop::State requested_state) { _requested_state = requested_state; _state_change_requested = true; }
        GameLoop::State get_requested_state() const { return _requested_state;}
    private:
        bool _state_change_requested = false;
        GameLoop::State _requested_state{GameLoop::State::CURRENT};
    };

    struct CheatConsole
    {
        static entt::entity create(const std::shared_ptr<Viewport>& viewport);
    };
}
