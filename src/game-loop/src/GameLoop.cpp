//
// Created by dbeef on 2/5/20.
//

#include "LevelGenerator.hpp"
#include "SplashScreenType.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "GameLoop.hpp"
#include "glad/glad.h"
#include "graphics_utils/DebugGlCall.hpp"
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "main-dude/MainDude.hpp"

#include <algorithm>

std::function<void(uint32_t delta_time_ms)>& GameLoop::get()
{
    return _loop;
}

GameLoop::GameLoop()
{
    TextureBank::instance().load_textures();
    TextureBank::instance().load_texture_regions();

    Camera::instance().update_gl_projection_matrix();

    // TODO: State pattern: "main menu" state should handle this, or "start game" because of also loading assets
    LevelGenerator::instance().getLevel().clean_map_layout();
    LevelGenerator::instance().getLevel().generate_frame();
    LevelGenerator::instance().getLevel().initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU);
    LevelGenerator::instance().getLevel().generate_cave_background();
    LevelGenerator::instance().getLevel().batch_vertices();
    LevelGenerator::instance().getLevel().add_render_entity();
    _game_objects.emplace_back(std::make_shared<MainDude>());

    _loop = [this](uint32_t delta_time_ms)
    {
        auto &camera = Camera::instance();
        auto &level_renderer = Renderer::instance();

        camera.update_gl_modelview_matrix();

        level_renderer.render();
        level_renderer.update();

        // Update game objects:

        for (auto& game_object : _game_objects)
        {
            game_object->update(delta_time_ms);
        }

        // Remove game objects marked for disposal:

        const auto it = std::remove_if(_game_objects.begin(), _game_objects.end(), [](const auto& game_object)
        {
            return game_object->is_marked_for_disposal();
        });

        if (it != _game_objects.end())
        {
            _game_objects.erase(it, _game_objects.end());
        }
    };
}
