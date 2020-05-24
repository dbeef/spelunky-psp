#include "LevelGenerator.hpp"
#include "logger/log.h"
#include "Renderer.hpp"
#include "GameLoop.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "GameLoopPlayingState.hpp"
#include "game-objects/GameObject.hpp"
#include "main-dude/MainDude.hpp"

GameLoopBaseState *GameLoopPlayingState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto &camera = Camera::instance();
    auto &level_renderer = Renderer::instance();
    auto& game_objects = game_loop._game_objects;

    auto x = game_loop._main_dude->get_x_pos_center();
    auto y = game_loop._main_dude->get_y_pos_center();
    camera.adjust_to_bounding_box(x, y);

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

    // Other:

    if (game_loop._main_dude->entered_door())
    {
        // re-enter playing state
        exit(game_loop);
        enter(game_loop);
        return this;
    }

    return this;
}

void GameLoopPlayingState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopPlayingState");

    LevelGenerator::instance().getLevel().clean_map_layout();
    LevelGenerator::instance().getLevel().generate_new_level_layout();
    LevelGenerator::instance().getLevel().initialise_tiles_from_room_layout();
    LevelGenerator::instance().getLevel().generate_frame();
    LevelGenerator::instance().getLevel().generate_cave_background();
    LevelGenerator::instance().getLevel().batch_vertices();
    LevelGenerator::instance().getLevel().add_render_entity();

    game_loop._main_dude = std::make_shared<MainDude>(0, 0);
    game_loop._game_objects.push_back(game_loop._main_dude);

    MapTile* entrance = nullptr;
    LevelGenerator::instance().getLevel().get_first_tile_of_given_type(MapTileType::ENTRANCE, entrance);
    assert(entrance);
    game_loop._main_dude->set_position_on_tile(entrance);
}

void GameLoopPlayingState::exit(GameLoop& game_loop)
{
    game_loop._game_objects = {};
    game_loop._main_dude = {};
}
