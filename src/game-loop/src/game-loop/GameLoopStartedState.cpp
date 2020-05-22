#include "GameLoopStartedState.hpp"

GameLoopBaseState *GameLoopStartedState::update(GameLoop &, uint32_t delta_time_ms)
{
    return this;
}

void GameLoopStartedState::enter(GameLoop &)
{

}

void GameLoopStartedState::exit(GameLoop &)
{

}
