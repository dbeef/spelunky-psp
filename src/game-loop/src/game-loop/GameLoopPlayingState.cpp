#include "LevelGenerator.hpp"
#include "logger/log.h"
#include "Renderer.hpp"
#include "GameLoop.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "GameLoopPlayingState.hpp"
#include "game-objects/GameObject.hpp"

GameLoopBaseState *GameLoopPlayingState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto &camera = Camera::instance();
    auto &level_renderer = Renderer::instance();
    auto& game_objects = game_loop._game_objects;

    camera.update_gl_modelview_matrix();

    level_renderer.render();
    level_renderer.update();

    // Update game objects:

    for (auto& game_object : game_objects)
    {
        game_object->update(delta_time_ms);
    }

    // Remove game objects marked for disposal:

    const auto it = std::remove_if(game_objects.begin(), game_objects.end(), [](const auto& game_object)
    {
        return game_object->is_marked_for_disposal();
    });

    if (it != game_objects.end())
    {
        game_objects.erase(it, game_objects.end());
    }

    return this;
}

void GameLoopPlayingState::enter(GameLoop&)
{
    log_info("Entered GameLoopPlayingState");

    LevelGenerator::instance().getLevel().clean_map_layout();
    LevelGenerator::instance().getLevel().generate_frame();
    LevelGenerator::instance().getLevel().generate_new_level_layout();
    LevelGenerator::instance().getLevel().initialise_tiles_from_room_layout();
    LevelGenerator::instance().getLevel().generate_cave_background();
    LevelGenerator::instance().getLevel().batch_vertices();
    LevelGenerator::instance().getLevel().add_render_entity();
}

void GameLoopPlayingState::exit(GameLoop &)
{

}
