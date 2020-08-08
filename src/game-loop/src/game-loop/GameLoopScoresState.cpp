#include "logger/log.h"
#include "ModelViewCamera.hpp"
#include "Renderer.hpp"
#include "GameLoopScoresState.hpp"
#include "GameLoop.hpp"
#include "Level.hpp"
#include "main-dude/MainDude.hpp"
#include "game-entities/ResetSign.hpp"
#include "game-entities/PauseOverlay.hpp"
#include "Input.hpp"

GameLoopBaseState *GameLoopScoresState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto& screen_space_camera = game_loop._cameras.screen_space;
    auto& model_view_camera = game_loop._cameras.model_view;
    auto& game_objects = game_loop._game_objects;
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

    // Update game objects:

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

    // TODO: Single point of tile width/height definition to not hardcode offset by magic values.
    game_loop._main_dude = std::make_shared<MainDude>(entrance->x + 0.5f, entrance->y + 0.5f);
    game_loop._game_objects.push_back(game_loop._main_dude);

    // Create text renderer:

    game_loop._text_buffer = std::make_shared<TextBuffer>();
    game_loop._game_objects.push_back(game_loop._text_buffer);

    // Create pause overlay:

    _pause_overlay = std::make_shared<PauseOverlay>(game_loop._viewport, PauseOverlay::Type::SCORES);
    _pause_overlay->set_text_buffer(game_loop._text_buffer);
    game_loop._game_objects.push_back(_pause_overlay);

    // Create reset sign:

     game_loop._game_objects.emplace_back(std::make_shared<ResetSign>(16.5f, 18.5f));
}

void GameLoopScoresState::exit(GameLoop& game_loop)
{
    _pause_overlay = nullptr;

    game_loop._game_objects = {};
    game_loop._main_dude = {};
    game_loop._text_buffer = {};
}
