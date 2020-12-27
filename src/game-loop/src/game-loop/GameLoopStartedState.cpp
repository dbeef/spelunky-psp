#include "game-loop/GameLoopStartedState.hpp"
#include "game-loop/GameLoop.hpp"

#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"

#include "Level.hpp"
#include "logger/log.h"
#include "system/GameEntitySystem.hpp"
#include "TextureBank.hpp"
#include "audio/Audio.hpp"
#include "main-dude/MainDude.hpp"
#include "other/World.hpp"
#include "TileBatch.hpp"

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

    Audio::instance().load();

    game_loop._cameras.model_view.calculate_coefficients();
    game_loop._cameras.screen_space.calculate_coefficients();

    TextureBank::instance().load_textures();
    TextureBank::instance().load_texture_regions();

    auto& tile_batch = game_loop._world->get_tile_batch();

    tile_batch->generate_cave_background();
    tile_batch->batch_vertices();
    tile_batch->add_render_entity();

    auto& main_dude = game_loop._world->get_main_dude();
    main_dude = std::make_shared<MainDude>(0, 0);

    _game_initialized = true;
}

void GameLoopStartedState::exit(GameLoop& game_loop)
{
    game_loop._game_entity_system->remove_all();
}
