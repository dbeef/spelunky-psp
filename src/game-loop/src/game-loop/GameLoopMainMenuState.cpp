#include "logger/log.h"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "GameLoopMainMenuState.hpp"
#include "GameLoop.hpp"
#include "LevelGenerator.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"
#include "game-objects/MainLogo.hpp"
#include "game-objects/QuitSign.hpp"

namespace
{
    // TODO: Remove once camera following main-dude is implemented.
    void handle_input()
    {
        const auto& input = Input::instance();
        Camera& camera = Camera::instance();

        if (input.left())
        {
            camera.setX(camera.getX() - 0.1f);
        }
        if (input.right())
        {
            camera.setX(camera.getX() + 0.1f);
        }
        if (input.up())
        {
            camera.setY(camera.getY() - 0.1f);
        }
        if (input.down())
        {
            camera.setY(camera.getY() + 0.1f);
        }
    }
}

GameLoopBaseState *GameLoopMainMenuState::update(GameLoop& game_loop, uint32_t delta_time_ms)
{
    auto &camera = Camera::instance();
    auto &level_renderer = Renderer::instance();
    auto& game_objects = game_loop._game_objects;
    
    camera.update_gl_modelview_matrix();

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

    handle_input();
    
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
    LevelGenerator::instance().getLevel().add_render_entity();

    auto &camera = Camera::instance();
    camera.setX(5.0f);
    camera.setY(7.2f);

    game_loop._game_objects.emplace_back(std::make_shared<MainDude>(17.37f, 18.5f));
    game_loop._game_objects.emplace_back(std::make_shared<MainLogo>(9.37f, 14.5f));
    game_loop._game_objects.emplace_back(std::make_shared<QuitSign>(16.0f, 9.5f));

    // TODO: Implement a mechanism for sprite rendering priority, so the main logo would be always rendered
    //       behind other sprites. Some RenderingPriority enum representing depth (Z axis) would be sufficient.
}

void GameLoopMainMenuState::exit(GameLoop &)
{

}
