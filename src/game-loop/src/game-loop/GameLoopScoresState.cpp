#include "game-loop/GameLoopScoresState.hpp"
#include "game-loop/GameLoop.hpp"

#include "game-entities/ResetSign.hpp"
#include "game-entities/PauseOverlay.hpp"
#include "game-entities/ScoresOverlay.hpp"
#include "main-dude/MainDude.hpp"
#include "system/GameEntitySystem.hpp"

#include "logger/log.h"
#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"
#include "Renderer.hpp"
#include "Level.hpp"

GameLoopBaseState *GameLoopScoresState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto& screen_space_camera = game_loop._cameras.screen_space;
    auto& model_view_camera = game_loop._cameras.model_view;
    auto& renderer = Renderer::instance();

    // Remove render entities marked for disposal:

    renderer.update();

    // Render:

    model_view_camera.update_gl_modelview_matrix();
    model_view_camera.update_gl_projection_matrix();

    renderer.render(Renderer::EntityType::MODEL_VIEW_SPACE);

    screen_space_camera.update_gl_modelview_matrix();
    screen_space_camera.update_gl_projection_matrix();

    renderer.render(Renderer::EntityType::SCREEN_SPACE);

    // Update game entities:

    if (_pause_overlay->is_paused())
    {
        if (_pause_overlay->is_quit_requested())
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
        return &game_loop._states.main_menu;
    }

    return this;
}

void GameLoopScoresState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopScoresState");

    Level::instance().get_tile_batch().generate_frame();
    Level::instance().get_tile_batch().initialise_tiles_from_splash_screen(SplashScreenType::SCORES);
    Level::instance().get_tile_batch().generate_cave_background();
    Level::instance().get_tile_batch().batch_vertices();

    // Splash screens are copied into the [0, 0] position (left-upper corner), center on them:
    auto &model_view_camera = game_loop._cameras.model_view;
    model_view_camera.set_x_not_rounded(game_loop._viewport->get_width_world_units() / 4.0f);
    model_view_camera.set_y_not_rounded(game_loop._viewport->get_height_world_units() / 4.0f);

    MapTile* entrance = nullptr;
    Level::instance().get_tile_batch().get_first_tile_of_given_type(MapTileType::EXIT, entrance);
    assert(entrance);

    // Update main dude:
    game_loop._main_dude->enter_standing_state();
    game_loop._main_dude->set_velocity(0, 0);
    game_loop._main_dude->set_position(entrance->x + (MapTile::PHYSICAL_WIDTH / 2.0f), entrance->y + (MapTile::PHYSICAL_HEIGHT / 2.0f));
    game_loop._game_entity_system->add(game_loop._main_dude);

    // Create pause overlay:
    _pause_overlay = std::make_shared<PauseOverlay>(game_loop._viewport, PauseOverlay::Type::SCORES);
    game_loop._game_entity_system->add(_pause_overlay);

    // Create scores overlay:
    _scores_overlay = std::make_shared<ScoresOverlay>(game_loop._viewport);
    game_loop._game_entity_system->add(_scores_overlay);

    // Create reset sign:
     game_loop._game_entity_system->add(std::make_shared<ResetSign>(16.5f, 18.5f));

    // Make main dude appear on the foreground:
    Renderer::instance().sort_by_layer();
}

void GameLoopScoresState::exit(GameLoop& game_loop)
{
    _pause_overlay = nullptr;
    _scores_overlay = nullptr;

    game_loop._game_entity_system->remove_all();
}
