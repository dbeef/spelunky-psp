#pragma once

#include <memory>
#include <entt/entt.hpp>
#include "game-loop/GameLoopBaseState.hpp"

class GameLoop;
class PauseOverlayComponent;
class ScoresOverlayComponent;

class GameLoopScoresState : public GameLoopBaseState
{
public:
    GameLoopBaseState* update(GameLoop&) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
private:
    entt::entity _main_dude = entt::null;
    entt::entity _pause_overlay = entt::null;
    entt::entity _death_overlay = entt::null;
};
