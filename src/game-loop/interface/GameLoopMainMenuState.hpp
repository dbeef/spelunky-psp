#pragma once

#include <memory>
#include "GameLoopBaseState.hpp"

class GameLoop;
class PauseScreen;

class GameLoopMainMenuState : public GameLoopBaseState
{
public:
    GameLoopBaseState* update(GameLoop&, uint32_t delta_time_ms) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
private:
    std::shared_ptr<PauseScreen> _pause = nullptr;
};
