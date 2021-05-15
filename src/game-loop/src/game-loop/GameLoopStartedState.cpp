#include "game-loop/GameLoopStartedState.hpp"
#include "game-loop/GameLoop.hpp"

#include "logger/log.h"
#include "TextureBank.hpp"
#include "audio/Audio.hpp"
#include "EntityRegistry.hpp"

#include "components/generic/MeshComponent.hpp"
#include "components/generic/QuadComponent.hpp"

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

    TextureBank::instance().load_textures();
    TextureBank::instance().load_texture_regions();

    auto& registry = EntityRegistry::instance().get_registry();
    registry.reserve<MeshComponent>(1024);
    registry.reserve<QuadComponent>(1024);

    _game_initialized = true;
}

void GameLoopStartedState::exit(GameLoop& game_loop)
{
}
