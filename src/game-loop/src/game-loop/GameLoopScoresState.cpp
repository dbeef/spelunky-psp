#include "logger/log.h"
#include "ModelViewCamera.hpp"
#include "Renderer.hpp"
#include "GameLoopScoresState.hpp"
#include "GameLoop.hpp"
#include "LevelGenerator.hpp"
#include "main-dude/MainDude.hpp"
#include "game-objects/ResetSign.hpp"
#include "game-objects/PauseScreen.hpp"
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

    if (_pause->is_paused())
    {
        if (_pause->is_quit_requested())
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
        return &game_loop._states.main_menu;
    }

    return this;
}

void GameLoopScoresState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopScoresState");

    LevelGenerator::instance().getLevel().clean_map_layout();
    LevelGenerator::instance().getLevel().generate_frame();
    LevelGenerator::instance().getLevel().initialise_tiles_from_splash_screen(SplashScreenType::SCORES);
    LevelGenerator::instance().getLevel().generate_cave_background();
    LevelGenerator::instance().getLevel().batch_vertices();

    auto &model_view_camera = game_loop._cameras.model_view;
    model_view_camera.set_x_not_rounded(5.0f);
    model_view_camera.set_y_not_rounded(7.0f);

    MapTile* entrance = nullptr;
    LevelGenerator::instance().getLevel().get_first_tile_of_given_type(MapTileType::EXIT, entrance);
    assert(entrance);

    // TODO: Single point of tile width/height definition to not hardcode offset by magic values.
    game_loop._main_dude = std::make_shared<MainDude>(entrance->x + 0.5f, entrance->y + 0.5f);
    game_loop._game_objects.push_back(game_loop._main_dude);

    // Create text renderer:

    game_loop._text_buffer = std::make_shared<TextBuffer>();
    game_loop._game_objects.push_back(game_loop._text_buffer);

    // Create Pause:

    _pause = std::make_shared<PauseScreen>(game_loop._viewport, PauseScreen::Type::SCORES);
    _pause->set_text_buffer(game_loop._text_buffer);
    game_loop._game_objects.push_back(_pause);

    // Create reset sign:

     game_loop._game_objects.emplace_back(std::make_shared<ResetSign>(16.5f, 18.5f));
}

void GameLoopScoresState::exit(GameLoop& game_loop)
{
    game_loop._game_objects = {};
    game_loop._main_dude = {};
    game_loop._text_buffer = {};
}
