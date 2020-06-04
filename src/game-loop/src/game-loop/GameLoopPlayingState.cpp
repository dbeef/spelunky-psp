#include "video/Context.hpp"
#include "LevelGenerator.hpp"
#include "Level.hpp"
#include "logger/log.h"
#include "Renderer.hpp"
#include "GameLoop.hpp"
#include "Input.hpp"
#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"
#include "GameLoopPlayingState.hpp"
#include "game-objects/GameObject.hpp"
#include "game-objects/HUD.hpp"
#include "main-dude/MainDude.hpp"
#include "game-objects/TextBuffer.hpp"

GameLoopBaseState *GameLoopPlayingState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto& model_view_camera = game_loop._cameras.model_view;
    auto& screen_space_camera = game_loop._cameras.screen_space;
    auto& renderer = Renderer::instance();
    auto& game_objects = game_loop._game_objects;

    // Adjust camera to follow main dude:

    auto x = game_loop._main_dude->get_x_pos_center();
    auto y = game_loop._main_dude->get_y_pos_center();
    model_view_camera.adjust_to_bounding_box(x, y);
    model_view_camera.adjust_to_level_boundaries(Consts::MAP_GAME_WIDTH_TILES, Consts::MAP_GAME_HEIGHT_TILES);

    // Remove render entities marked for disposal:

    renderer.update();

    // Render:

    model_view_camera.update_gl_modelview_matrix();
    model_view_camera.update_gl_projection_matrix();

    renderer.render(Renderer::EntityType::MODEL_VIEW_SPACE);

    screen_space_camera.update_gl_modelview_matrix();
    screen_space_camera.update_gl_projection_matrix();

    renderer.render(Renderer::EntityType::SCREEN_SPACE);

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
        return &game_loop._states.level_summary;
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

    // Create main dude:

    game_loop._main_dude = std::make_shared<MainDude>(0, 0);
    game_loop._game_objects.push_back(game_loop._main_dude);

    MapTile* entrance = nullptr;
    LevelGenerator::instance().getLevel().get_first_tile_of_given_type(MapTileType::ENTRANCE, entrance);
    assert(entrance);
    game_loop._main_dude->set_position_on_tile(entrance);

    // Create hud:

    const auto hud_pos_x = static_cast<float>(Video::instance().get_window_width() * 0.05f);
    const auto hud_pos_y = static_cast<float>(Video::instance().get_window_height() * 0.05f);

    game_loop._game_objects.push_back(std::make_shared<HUD>(hud_pos_x, hud_pos_y));

    // Create text renderer:

    game_loop._text_buffer = std::make_shared<TextBuffer>();
    game_loop._game_objects.push_back(game_loop._text_buffer);

    auto id = game_loop._text_buffer->create_text(4);
    game_loop._text_buffer->update_text(id, {100, 100}, "DUPA", 4);
}

void GameLoopPlayingState::exit(GameLoop& game_loop)
{
    game_loop._game_objects = {};
    game_loop._main_dude = {};
    game_loop._text_buffer = {};
}
