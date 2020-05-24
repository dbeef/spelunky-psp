#pragma once

#include "GameLoopBaseState.hpp"
#include <memory>

class GameLoop;
class MainDude;

class GameLoopMainMenuState : public GameLoopBaseState
{
public:
    GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
};

