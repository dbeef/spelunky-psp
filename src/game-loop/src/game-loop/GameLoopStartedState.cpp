#include "LevelGenerator.hpp"
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

void GameLoopStartedState::enter(GameLoop &)
{
    log_info("Entered GameLoopStartedState");

    // Need to be done only once in whole game lifespan:

    TextureBank::instance().load_textures();
    TextureBank::instance().load_texture_regions();

    ModelViewCamera::instance().calculate_coefficients();
    ScreenSpaceCamera::instance().calculate_coefficients();

    LevelGenerator::instance().getLevel().clean_map_layout();
    LevelGenerator::instance().getLevel().generate_cave_background();
    LevelGenerator::instance().getLevel().batch_vertices();
    LevelGenerator::instance().getLevel().add_render_entity();

    _game_initialized = true;
}

void GameLoopStartedState::exit(GameLoop &)
{

}
