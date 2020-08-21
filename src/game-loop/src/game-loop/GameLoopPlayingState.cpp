#include "Level.hpp"
#include "TileBatch.hpp"
#include "logger/log.h"
#include "Renderer.hpp"
#include "GameLoop.hpp"
#include "Input.hpp"
#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"
#include "GameLoopPlayingState.hpp"
#include "system/GameEntitySystem.hpp"
#include "game-entities/GameEntity.hpp"
#include "game-entities/HUD.hpp"
#include "game-entities/PauseOverlay.hpp"
#include "game-entities/DeathOverlay.hpp"
#include "main-dude/MainDude.hpp"

GameLoopBaseState *GameLoopPlayingState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto& model_view_camera = game_loop._cameras.model_view;
    auto& screen_space_camera = game_loop._cameras.screen_space;
    auto& renderer = Renderer::instance();

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

    if (_pause_overlay->is_paused())
    {
        if (_pause_overlay->is_death_requested())
        {
            log_info("Death requested.");
            game_loop._main_dude->enter_dead_state();
            _death_overlay->launch();
            _death_overlay->enable_input();
            _pause_overlay->reset();
            _pause_overlay->disable_input();
        }
        else if (_pause_overlay->is_quit_requested())
        {
            log_info("Quit requested.");
            game_loop._exit = true;
        }

        _pause_overlay->update(delta_time_ms);
    }
    else
    {
        game_loop._game_entity_system->update(delta_time_ms);
    }

    // Other:

    if (game_loop._main_dude->entered_door())
    {
        return &game_loop._states.level_summary;
    }

    if (_death_overlay->is_scores_requested())
    {
        return &game_loop._states.scores;
    }

    return this;
}

void GameLoopPlayingState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopPlayingState");

    Level::instance().get_tile_batch().generate_new_level_layout();
    Level::instance().get_tile_batch().initialise_tiles_from_room_layout();
    Level::instance().get_tile_batch().generate_frame();
    Level::instance().get_tile_batch().generate_cave_background();
    Level::instance().get_tile_batch().batch_vertices();

    // Create main dude:

    game_loop._main_dude = std::make_shared<MainDude>(0, 0);
    game_loop._game_entity_system->add(game_loop._main_dude);

    MapTile *entrance = nullptr;
    Level::instance().get_tile_batch().get_first_tile_of_given_type(MapTileType::ENTRANCE, entrance);
    assert(entrance);
    game_loop._main_dude->set_position_on_tile(entrance);

    // Create HUD:

    auto hud = std::make_shared<HUD>(game_loop._viewport, game_loop._main_dude);
    game_loop._game_entity_system->add(hud);

    // Create pause overlay:

    _pause_overlay = std::make_shared<PauseOverlay>(game_loop._viewport, PauseOverlay::Type::PLAYING);
    game_loop._game_entity_system->add(_pause_overlay);
    
    // Create death overlay:

    _death_overlay = std::make_shared<DeathOverlay>(game_loop._viewport);
    _death_overlay->disable_input();
    game_loop._game_entity_system->add(_death_overlay);
}

void GameLoopPlayingState::exit(GameLoop& game_loop)
{
    _death_overlay = nullptr;
    _pause_overlay = nullptr;

    game_loop._game_entity_system->remove_all();
    game_loop._main_dude = {};
}
