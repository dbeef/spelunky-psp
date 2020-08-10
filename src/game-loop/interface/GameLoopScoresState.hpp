#pragma once

#include <memory>
#include "GameLoopBaseState.hpp"

class GameLoop;
class PauseOverlay;
class ScoresOverlay;

class GameLoopScoresState : public GameLoopBaseState
{
public:
    GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
private:
    std::shared_ptr<PauseOverlay> _pause_overlay = nullptr;
    std::shared_ptr<ScoresOverlay> _scores_overlay = nullptr;
};
