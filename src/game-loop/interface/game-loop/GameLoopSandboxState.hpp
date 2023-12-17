#pragma once

#include <memory>
#include <entt/entt.hpp>
#include "game-loop/GameLoopBaseState.hpp"

class GameLoop;
class PauseOverlayComponent;
class ScoresOverlayComponent;

class GameLoopSandboxState : public GameLoopBaseState
{
public:
    GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
private:
    entt::entity _main_dude = entt::null;
    entt::entity _pause_overlay = entt::null;
    entt::entity _death_overlay = entt::null;
    entt::entity _cheat_console = entt::null;
};
