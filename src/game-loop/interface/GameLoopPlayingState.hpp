#pragma once

#include "GameLoopBaseState.hpp"

class GameLoop;

class GameLoopPlayingState : public GameLoopBaseState
{
public:
    GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
};

