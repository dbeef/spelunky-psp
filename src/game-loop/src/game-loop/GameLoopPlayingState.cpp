#include "logger/log.h"
#include "Renderer.hpp"
#include "GameLoop.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "GameLoopPlayingState.hpp"
#include "game-objects/GameObject.hpp"

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

GameLoopBaseState *GameLoopPlayingState::update(GameLoop& game_loop, uint32_t delta_time_ms)
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

void GameLoopPlayingState::enter(GameLoop&)
{
    log_info("Entered GameLoopPlayingState");
}

void GameLoopPlayingState::exit(GameLoop &)
{

}
