#include "logger/log.h"
#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"
#include "Renderer.hpp"
#include "GameLoopLevelSummaryState.hpp"
#include "GameLoop.hpp"
#include "LevelGenerator.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"
#include "game-objects/MainLogo.hpp"
#include "game-objects/QuitSign.hpp"
#include "game-objects/StartSign.hpp"
#include "game-objects/ScoresSign.hpp"
#include "game-objects/TutorialSign.hpp"
#include "game-objects/CopyrightsSign.hpp"

GameLoopBaseState *GameLoopLevelSummaryState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto &model_view_camera = game_loop._cameras.model_view;
    auto &screen_space_camera = game_loop._cameras.screen_space;
    auto &level_renderer = Renderer::instance();
    auto& game_objects = game_loop._game_objects;

    model_view_camera.update_gl_modelview_matrix();
    model_view_camera.update_gl_projection_matrix();

    level_renderer.render();
    level_renderer.update();

    screen_space_camera.update_gl_modelview_matrix();
    screen_space_camera.update_gl_projection_matrix();

    // TODO: Render HUD

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
        return &game_loop._states.playing;
    }

    return this;
}

void GameLoopLevelSummaryState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopLevelSummaryState");

    LevelGenerator::instance().getLevel().clean_map_layout();
    LevelGenerator::instance().getLevel().generate_frame();
    LevelGenerator::instance().getLevel().initialise_tiles_from_splash_screen(SplashScreenType::LEVEL_SUMMARY);
    LevelGenerator::instance().getLevel().generate_cave_background();
    LevelGenerator::instance().getLevel().batch_vertices();

    auto &model_view_camera = game_loop._cameras.model_view;
    model_view_camera.set_x_not_rounded(5.0f);
    model_view_camera.set_y_not_rounded(7.0f);

    game_loop._main_dude = std::make_shared<MainDude>(0, 0);
    game_loop._game_objects.push_back(game_loop._main_dude);

    MapTile* entrance = nullptr;
    LevelGenerator::instance().getLevel().get_first_tile_of_given_type(MapTileType::ENTRANCE, entrance);
    assert(entrance);
    game_loop._main_dude->set_position_on_tile(entrance);
    game_loop._main_dude->enter_level_summary_state();
}

void GameLoopLevelSummaryState::exit(GameLoop& game_loop)
{
    game_loop._game_objects = {};
    game_loop._main_dude = {};
}
