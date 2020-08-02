#include "Level.hpp"
#include "logger/log.h"
#include "GameLoopStartedState.hpp"
#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"
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

void GameLoopStartedState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopStartedState");

    // Need to be done only once in whole game lifespan:

    game_loop._cameras.model_view.calculate_coefficients();
    game_loop._cameras.screen_space.calculate_coefficients();

    TextureBank::instance().load_textures();
    TextureBank::instance().load_texture_regions();

    Level::instance().get_tile_batch().generate_cave_background();
    Level::instance().get_tile_batch().batch_vertices();
    Level::instance().get_tile_batch().add_render_entity();

    _game_initialized = true;
}

void GameLoopStartedState::exit(GameLoop& game_loop)
{
    game_loop._game_objects = {};
    game_loop._main_dude = {};
    game_loop._text_buffer = {};
}
