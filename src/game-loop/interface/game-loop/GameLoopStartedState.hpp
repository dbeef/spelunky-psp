#pragma once

#include "game-loop/GameLoopBaseState.hpp"

class GameLoop;

class GameLoopStartedState : public GameLoopBaseState
{
public:
    GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
private:
    bool _game_initialized = false;
};

