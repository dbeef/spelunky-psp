#include "GameLoopLevelSummaryState.hpp"
#include "GameLoop.hpp"

#include "game-entities/MainLogo.hpp"
#include "game-entities/QuitSign.hpp"
#include "game-entities/StartSign.hpp"
#include "game-entities/ScoresSign.hpp"
#include "game-entities/TutorialSign.hpp"
#include "game-entities/CopyrightsSign.hpp"
#include "game-entities/LevelSummaryOverlay.hpp"
#include "system/GameEntitySystem.hpp"
#include "main-dude/MainDude.hpp"

#include "logger/log.h"
#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"
#include "Renderer.hpp"
#include "Level.hpp"

GameLoopBaseState *GameLoopLevelSummaryState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto& model_view_camera = game_loop._cameras.model_view;
    auto& screen_space_camera = game_loop._cameras.screen_space;
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

    game_loop._game_entity_system->update(delta_time_ms);

    // Other:

    if (game_loop._main_dude->entered_door())
    {
        return &game_loop._states.playing;
    }

    return this;
}

void GameLoopLevelSummaryState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopLevelSummaryState");

    Level::instance().get_tile_batch().generate_frame();
    Level::instance().get_tile_batch().initialise_tiles_from_splash_screen(SplashScreenType::LEVEL_SUMMARY);
    Level::instance().get_tile_batch().generate_cave_background();
    Level::instance().get_tile_batch().batch_vertices();

    // Splash screens are copied into the [0, 0] position (left-upper corner), center on them:
    auto &model_view_camera = game_loop._cameras.model_view;
    model_view_camera.set_x_not_rounded(game_loop._viewport->get_width_world_units() / 4.0f);
    model_view_camera.set_y_not_rounded(game_loop._viewport->get_height_world_units() / 4.0f);

    // Update main dude:
    MapTile* entrance = nullptr;
    Level::instance().get_tile_batch().get_first_tile_of_given_type(MapTileType::ENTRANCE, entrance);
    assert(entrance);
    game_loop._main_dude->set_position_on_tile(entrance);
    game_loop._main_dude->enter_level_summary_state();
    game_loop._main_dude->set_velocity(0, 0);
    game_loop._game_entity_system->add(game_loop._main_dude);

    // Create level summary overlay:

    _level_summary_overlay = std::make_shared<LevelSummaryOverlay>(game_loop._viewport);
    game_loop._game_entity_system->add(_level_summary_overlay);

    // Make main dude appear on the foreground:
    Renderer::instance().sort_by_layer();
}

void GameLoopLevelSummaryState::exit(GameLoop& game_loop)
{
    _level_summary_overlay = nullptr;
    game_loop._game_entity_system->remove_all();
}
