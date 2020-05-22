//
// Created by dbeef on 2/5/20.
//

#ifndef RESOURCE_COMPILER_GAMELOOP_HPP
#define RESOURCE_COMPILER_GAMELOOP_HPP

#include <functional>
#include <memory>
#include <vector>

#include "GameLoopBaseState.hpp"
#include "GameLoopMainMenuState.hpp"
#include "GameLoopPlayingState.hpp"

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

    struct
    {
        GameLoopMainMenuState main_menu;
        GameLoopPlayingState playing;
    } _states;

    std::vector<std::shared_ptr<GameObject>> _game_objects;
    std::function<void(uint32_t delta_time_ms)> _loop;
};


#endif //RESOURCE_COMPILER_GAMELOOP_HPP
