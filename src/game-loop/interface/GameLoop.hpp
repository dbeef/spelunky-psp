#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "viewport/Viewport.hpp"

#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"

#include "GameLoopBaseState.hpp"
#include "GameLoopLevelSummaryState.hpp"
#include "GameLoopMainMenuState.hpp"
#include "GameLoopPlayingState.hpp"
#include "GameLoopStartedState.hpp"

class GameObject;
class MainDude;
class TextBuffer;
class Viewport;

class GameLoop
{
public:
    GameLoop(std::shared_ptr<Viewport>);
    std::function<void(uint32_t delta_time_ms)>& get();
private:

    friend class GameLoopBaseState;
    friend class GameLoopMainMenuState;
    friend class GameLoopPlayingState;
    friend class GameLoopStartedState;
    friend class GameLoopLevelSummaryState;

    struct
    {
        GameLoopMainMenuState main_menu;
        GameLoopPlayingState playing;
        GameLoopStartedState started;
        GameLoopLevelSummaryState level_summary;
        GameLoopBaseState* current;
    } _states;
    
    struct
    {
        ModelViewCamera model_view;
        ScreenSpaceCamera screen_space;
    } _cameras;

    std::shared_ptr<Viewport> _viewport;
    std::shared_ptr<TextBuffer> _text_buffer;
    std::shared_ptr<MainDude> _main_dude;
    std::vector<std::shared_ptr<GameObject>> _game_objects;
    std::function<void(uint32_t delta_time_ms)> _loop;
};
