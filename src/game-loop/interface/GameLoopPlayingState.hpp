#pragma once

#include <memory>

#include "GameLoopBaseState.hpp"
#include "patterns/Observer.hpp"

class GameLoop;
class HUD;
class PauseOverlay;
class DeathOverlay;
enum class MainDudeEvent;

class GameLoopPlayingState : public GameLoopBaseState, public Observer<MainDudeEvent>
{
public:
    GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
    void on_notify(const MainDudeEvent*) override;
private:
    std::shared_ptr<HUD> _hud = nullptr;
    std::shared_ptr<PauseOverlay> _pause_overlay = nullptr;
    std::shared_ptr<DeathOverlay> _death_overlay = nullptr;
};
