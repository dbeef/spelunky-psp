#include "GameLoop.hpp"

#include <algorithm>
#include <cassert>

std::function<bool(uint32_t delta_time_ms)>& GameLoop::get()
{
    return _loop;
}

GameLoop::GameLoop(const std::shared_ptr<Viewport>& viewport)
    : _viewport(viewport)
    , _cameras{{viewport}, {viewport}}
{
    _states.current = &_states.started;
    _states.current->enter(*this);

    _loop = [this](uint32_t delta_time_ms)
    {
        assert(_states.current);
        auto new_state = _states.current->update(*this, delta_time_ms);

        if (new_state != _states.current)
        {
            _states.current->exit(*this);
            new_state->enter(*this);
            _states.current = new_state;
        }

        return _exit;
    };
}
