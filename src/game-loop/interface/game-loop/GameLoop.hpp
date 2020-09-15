#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"

#include "game-loop/GameLoopBaseState.hpp"
#include "game-loop/GameLoopLevelSummaryState.hpp"
#include "game-loop/GameLoopMainMenuState.hpp"
#include "game-loop/GameLoopPlayingState.hpp"
#include "game-loop/GameLoopStartedState.hpp"
#include "game-loop/GameLoopScoresState.hpp"

class GameEntitySystem;
class MainDude;
class Viewport;

class GameLoop
{
public:
    GameLoop(const std::shared_ptr<Viewport>&);
    std::function<bool(uint32_t delta_time_ms)>& get();
private:

    friend class GameLoopBaseState;
    friend class GameLoopMainMenuState;
    friend class GameLoopPlayingState;
    friend class GameLoopStartedState;
    friend class GameLoopLevelSummaryState;
    friend class GameLoopScoresState;

    struct
    {
        GameLoopMainMenuState main_menu;
        GameLoopPlayingState playing;
        GameLoopStartedState started;
        GameLoopLevelSummaryState level_summary;
        GameLoopScoresState scores;
        GameLoopBaseState* current;
    } _states;
    
    struct
    {
        ModelViewCamera model_view;
        ScreenSpaceCamera screen_space;
    } _cameras;

    bool _exit = false;

    std::shared_ptr<GameEntitySystem> _game_entity_system;
    std::shared_ptr<Viewport> _viewport;
    std::shared_ptr<MainDude> _main_dude;
    std::function<bool(uint32_t delta_time_ms)> _loop;
};
