#pragma once

#include <memory>

#include "game-loop/GameLoopBaseState.hpp"
#include "patterns/Observer.hpp"

class GameLoop;
class HUDComponent;
class PauseOverlayComponent;
class DeathOverlayComponent;
enum class MainDudeEvent;

class GameLoopPlayingState : public GameLoopBaseState, public Observer<MainDudeEvent>
{
public:
    GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
    void on_notify(const MainDudeEvent*) override;
private:
    entt::entity _main_dude = entt::null;
    entt::entity _hud = entt::null;
    entt::entity _pause_overlay = entt::null;
    entt::entity _death_overlay = entt::null;
};
