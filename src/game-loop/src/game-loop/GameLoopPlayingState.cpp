#include "Level.hpp"
#include "TileBatch.hpp"
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
#include "game-objects/PauseScreen.hpp"

#include <ctime>

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
    model_view_camera.adjust_to_level_boundaries(Consts::LEVEL_WIDTH_TILES, Consts::LEVEL_HEIGHT_TILES);

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

    if (_pause->is_paused())
    {
        if (_pause->is_death_requested())
        {
            log_info("Death requested.");
            // TODO: Kill the main dude.
            _pause->unpause();
        }
        else if (_pause->is_quit_requested())
        {
            log_info("Quit requested.");
            _pause->unpause();
            game_loop._exit = true;
        }
        _pause->update(delta_time_ms);
    }
    else
    {
        for (auto &game_object : game_objects)
        {
            game_object->update(delta_time_ms);
        }
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

    std::srand(std::time(0));

    Level::instance().get_tile_batch().clean_map_layout();
    Level::instance().get_tile_batch().generate_new_level_layout();
    Level::instance().get_tile_batch().initialise_tiles_from_room_layout();
    Level::instance().get_tile_batch().generate_frame();
    Level::instance().get_tile_batch().generate_cave_background();
    Level::instance().get_tile_batch().batch_vertices();

    // Create main dude:

    game_loop._main_dude = std::make_shared<MainDude>(0, 0);
    game_loop._game_objects.push_back(game_loop._main_dude);

    MapTile *entrance = nullptr;
    Level::instance().get_tile_batch().get_first_tile_of_given_type(MapTileType::ENTRANCE, entrance);
    assert(entrance);
    game_loop._main_dude->set_position_on_tile(entrance);

    // Create text renderer:

    game_loop._text_buffer = std::make_shared<TextBuffer>();
    game_loop._game_objects.push_back(game_loop._text_buffer);

    // Create HUD:

    auto hud = std::make_shared<HUD>(game_loop._viewport);
    game_loop._game_objects.push_back(hud);

    hud->set_text_buffer(game_loop._text_buffer);
    hud->set_bombs_count(4);
    hud->set_dollars_count(0);
    hud->set_hearts_count(4);
    hud->set_ropes_count(4);

    // Create Pause:

    _pause = std::make_shared<PauseScreen>(game_loop._viewport, PauseScreen::Type::PLAYING);
    _pause->set_text_buffer(game_loop._text_buffer);
    game_loop._game_objects.push_back(_pause);
}

void GameLoopPlayingState::exit(GameLoop& game_loop)
{
    game_loop._game_objects = {};
    game_loop._main_dude = {};
    game_loop._text_buffer = {};
}
