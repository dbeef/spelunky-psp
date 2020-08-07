#pragma once

#include <memory>
#include "GameLoopBaseState.hpp"

class GameLoop;
class PauseOverlay;
class DeathOverlay;

class GameLoopPlayingState : public GameLoopBaseState
{
public:
    GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
private:
    std::shared_ptr<PauseOverlay> _pause_overlay = nullptr;
    std::shared_ptr<DeathOverlay> _death_overlay = nullptr;
};
