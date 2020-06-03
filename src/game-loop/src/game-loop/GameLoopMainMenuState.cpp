#include "logger/log.h"
#include "ModelViewCamera.hpp"
#include "Renderer.hpp"
#include "GameLoopMainMenuState.hpp"
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

GameLoopBaseState *GameLoopMainMenuState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto &model_view_camera = ModelViewCamera::instance();
    auto &level_renderer = Renderer::instance();
    auto& game_objects = game_loop._game_objects;

    model_view_camera.update_gl_modelview_matrix();
    model_view_camera.update_gl_projection_matrix();

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
        return &game_loop._states.playing;
    }

    return this;
}

void GameLoopMainMenuState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopMainMenuState");

    LevelGenerator::instance().getLevel().clean_map_layout();
    LevelGenerator::instance().getLevel().generate_frame();
    LevelGenerator::instance().getLevel().initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU);
    LevelGenerator::instance().getLevel().generate_cave_background();
    LevelGenerator::instance().getLevel().batch_vertices();

    auto &camera = ModelViewCamera::instance();
    camera.set_x_not_rounded(5.0f);
    camera.set_y_not_rounded(7.0f);

    game_loop._game_objects.emplace_back(std::make_shared<MainLogo>(9.7f, 13.5f));
    game_loop._game_objects.emplace_back(std::make_shared<QuitSign>(16.0f, 9.5f));
    game_loop._game_objects.emplace_back(std::make_shared<StartSign>(5.5f, 17.0f));
    game_loop._game_objects.emplace_back(std::make_shared<ScoresSign>(9.5f, 17.0f));
    game_loop._game_objects.emplace_back(std::make_shared<TutorialSign>(1.0f, 16.5f));
    game_loop._game_objects.emplace_back(std::make_shared<CopyrightsSign>(10.0f, 18.75f));

    game_loop._main_dude = std::make_shared<MainDude>(17.45f, 8.5f);
    game_loop._game_objects.push_back(game_loop._main_dude);

    // TODO: Implement a mechanism for sprite rendering priority, so the main logo would be always rendered
    //       behind other sprites. Some RenderingPriority enum representing depth (Z axis) would be sufficient.
    //       Right now stacking logo and signs before the main dude handles the problem.
}

void GameLoopMainMenuState::exit(GameLoop& game_loop)
{
    game_loop._game_objects = {};
    game_loop._main_dude = {};
}
