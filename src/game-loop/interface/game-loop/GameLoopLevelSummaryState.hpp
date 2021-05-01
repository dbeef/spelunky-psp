#pragma once

#include "game-loop/GameLoopBaseState.hpp"
#include <memory>
#include <entt/entt.hpp>

class GameLoop;
class MainDudeComponent;
class LevelSummaryOverlayComponent;

class GameLoopLevelSummaryState : public GameLoopBaseState
{
public:
    GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
private:
    entt::entity _damsel = entt::null;
    entt::entity _main_dude = entt::null;
    entt::entity _pause_overlay = entt::null;
    entt::entity _death_overlay = entt::null;
};

