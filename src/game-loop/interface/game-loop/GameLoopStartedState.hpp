#pragma once

#include "game-loop/GameLoopBaseState.hpp"

class GameLoop;

class GameLoopStartedState : public GameLoopBaseState
{
public:
    GameLoopBaseState* update(GameLoop&) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
private:
    bool _game_initialized = false;
};

