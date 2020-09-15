#include "game-loop/GameLoopMainMenuState.hpp"
#include "game-loop/GameLoop.hpp"

#include "game-entities/MainLogo.hpp"
#include "game-entities/PauseOverlay.hpp"
#include "game-entities/QuitSign.hpp"
#include "game-entities/StartSign.hpp"
#include "game-entities/ScoresSign.hpp"
#include "game-entities/TutorialSign.hpp"
#include "game-entities/CopyrightsSign.hpp"
#include "system/GameEntitySystem.hpp"
#include "main-dude/MainDude.hpp"

#include "logger/log.h"
#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"
#include "Renderer.hpp"
#include "Level.hpp"
#include "audio/Audio.hpp"

#include <cmath>

namespace
{
    const Point2D PLAY_COORDS = {5, 9};
    const Point2D SCORES_COORDS = {9, 9};
    const Point2D TUTORIAL_COORDS = {1, 9};
}

GameLoopBaseState *GameLoopMainMenuState::update(GameLoop& game_loop, uint32_t delta_time_ms)
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
        const Point2D pos_in_tiles = {std::floor(game_loop._main_dude->get_x_pos_center()),
                                      std::floor(game_loop._main_dude->get_y_pos_center())};
        if (pos_in_tiles == PLAY_COORDS)
        {
            return &game_loop._states.playing;
        }
        else if (pos_in_tiles == SCORES_COORDS)
        {
            return &game_loop._states.scores;
        }
        else
        {
            assert(false);
        }
    }

    return this;
}

void GameLoopMainMenuState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopMainMenuState");

    Audio::instance().play(MusicType::TITLE);

    Level::instance().get_tile_batch().generate_frame();
    Level::instance().get_tile_batch().initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU);
    Level::instance().get_tile_batch().generate_cave_background();
    Level::instance().get_tile_batch().batch_vertices();

    // Splash screens are copied into the [0, 0] position (left-upper corner), center on them:
    auto &model_view_camera = game_loop._cameras.model_view;
    model_view_camera.set_x_not_rounded(game_loop._viewport->get_width_world_units() / 4.0f);
    model_view_camera.set_y_not_rounded(game_loop._viewport->get_height_world_units() / 4.0f);

    game_loop._game_entity_system->add(std::make_shared<MainLogo>(9.7f, 5.5f));
    game_loop._game_entity_system->add(std::make_shared<StartSign>(5.5f, 9.0f));
    game_loop._game_entity_system->add(std::make_shared<ScoresSign>(9.5f, 9.0f));
    game_loop._game_entity_system->add(std::make_shared<TutorialSign>(1.0f, 8.5f));
    game_loop._game_entity_system->add(std::make_shared<CopyrightsSign>(10.0f, 10.75f));
    game_loop._game_entity_system->add(std::make_shared<QuitSign>(16.0f, 1.5f));

    // Update main dude:
    game_loop._main_dude->enter_standing_state();
    game_loop._main_dude->set_velocity(0, 0);
    game_loop._main_dude->set_position(17.45f, 8.5f);
    game_loop._game_entity_system->add(game_loop._main_dude);

    // Create pause overlay:
    _pause_overlay = std::make_shared<PauseOverlay>(game_loop._viewport, PauseOverlay::Type::MAIN_MENU);
    game_loop._game_entity_system->add(_pause_overlay);

    // Make main dude appear on the foreground:
    Renderer::instance().sort_by_layer();
}

void GameLoopMainMenuState::exit(GameLoop& game_loop)
{
    Audio::instance().stop();

    _pause_overlay = nullptr;
    game_loop._game_entity_system->remove_all();
}
