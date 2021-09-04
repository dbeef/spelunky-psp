#pragma once

#include <cstdint>

class GameLoop;

class GameLoopBaseState
{
public:
    ~GameLoopBaseState() = default;
    virtual GameLoopBaseState* update(GameLoop&) = 0;
    virtual void enter(GameLoop&) = 0;
    virtual void exit(GameLoop&) = 0;
};
