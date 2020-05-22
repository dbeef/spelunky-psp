#pragma once

#include "GameLoopBaseState.hpp"
#include <cstdint>

class GameLoop;

class GameLoopStartedState : public GameLoopBaseState
{
public:
    GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
};

