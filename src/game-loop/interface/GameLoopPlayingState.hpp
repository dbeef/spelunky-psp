#pragma once

#include "GameLoopBaseState.hpp"

class GameLoop;
class PausePlayingGame;

class GameLoopPlayingState : public GameLoopBaseState
{
public:
    GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
private:
    std::shared_ptr<PausePlayingGame> _pause = nullptr;
};

