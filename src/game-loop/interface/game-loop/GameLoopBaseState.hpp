#pragma once

#include <cstdint>

class GameLoop;

// TODO: Rename to IGameLoopState to not confuse with GameLoopState enum
class GameLoopBaseState
{
public:
    ~GameLoopBaseState() = default;
    virtual GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) = 0;
    virtual void enter(GameLoop&) = 0;
    virtual void exit(GameLoop&) = 0;
};
