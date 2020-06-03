#include "LevelGenerator.hpp"
#include "SplashScreenType.hpp"
#include "ModelViewCamera.hpp"
#include "Input.hpp"
#include "GameLoop.hpp"
#include "glad/glad.h"
#include "graphics_utils/DebugGlCall.hpp"
#include "Renderer.hpp"
#include "game-objects/GameObject.hpp"
#include "main-dude/MainDude.hpp"

#include <algorithm>

std::function<void(uint32_t delta_time_ms)>& GameLoop::get()
{
    return _loop;
}

GameLoop::GameLoop()
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
    };
}
