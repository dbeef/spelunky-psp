#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "GameLoopBaseState.hpp"
#include "GameLoopMainMenuState.hpp"
#include "GameLoopPlayingState.hpp"
#include "GameLoopStartedState.hpp"

class GameObject;

class GameLoop
{
public:
    GameLoop();
    std::function<void(uint32_t delta_time_ms)>& get();
private:

    friend class GameLoopBaseState;
    friend class GameLoopMainMenuState;
    friend class GameLoopPlayingState;
    friend class GameLoopStartedState;

    struct
    {
        GameLoopMainMenuState main_menu;
        GameLoopPlayingState playing;
        GameLoopStartedState started;
        GameLoopBaseState* current;
    } _states;

    std::vector<std::shared_ptr<GameObject>> _game_objects;
    std::function<void(uint32_t delta_time_ms)> _loop;
};
