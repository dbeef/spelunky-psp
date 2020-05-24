#include "logger/log.h"
#include "GameLoopStartedState.hpp"
#include "Camera.hpp"
#include "TextureBank.hpp"
#include "GameLoop.hpp"

GameLoopBaseState *GameLoopStartedState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    if (!_game_initialized)
    {
        return this;
    }
    else
    {
        return &game_loop._states.main_menu;
    }
}

void GameLoopStartedState::enter(GameLoop &)
{
    log_info("Entered GameLoopStartedState");

    // Need to be done only once in whole game lifespan:

    TextureBank::instance().load_textures();
    TextureBank::instance().load_texture_regions();

    Camera::instance().calculate_coefficients();
    Camera::instance().update_gl_projection_matrix();

    _game_initialized = true;
}

void GameLoopStartedState::exit(GameLoop &)
{

}
