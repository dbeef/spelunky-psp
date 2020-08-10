#pragma once

#include "GameLoopBaseState.hpp"
#include <memory>

class GameLoop;
class MainDude;
class LevelSummaryOverlay;

class GameLoopLevelSummaryState : public GameLoopBaseState
{
public:
    GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
private:
    std::shared_ptr<LevelSummaryOverlay> _level_summary_overlay = nullptr;
};

