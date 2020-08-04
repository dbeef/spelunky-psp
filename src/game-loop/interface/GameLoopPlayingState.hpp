#pragma once

#include <memory>
#include <cstdint>

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

    // Or should HUD store this?
    struct
    {
        std::uint16_t hearts = 4;
        std::uint16_t bombs = 4;
        std::uint16_t ropes = 4;
        std::uint16_t gold = 0;
    } _status;

    // Possible places to store status:
    //
    // - GameLoopPlayingState
    // - HUD
    // - MainDude
    //
    // MainDude would be easiest, yet HUD still needs to be informed about the amounts.
    // Could pass HUD sptr to main dude.
    //
    // If placed in the GameLoopPlayingState, GameLoopPlayingState would be the middle man having
    // pointers to both sides. HUD could store its copy of MainDude's state amounts, and game loop playing state would
    // update them.
    //
    // Or, MainDude could store the information and HUD could get a pointer to the main dude instance, updating
    // amounts when they change.
    //
    // Or... Event system? Subscription on main dude's gold/hearts/ropes/bombes count changed?

    std::shared_ptr<PauseOverlay> _pause_overlay = nullptr;
    std::shared_ptr<DeathOverlay> _death_overlay = nullptr;
};
