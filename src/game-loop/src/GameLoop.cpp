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

namespace
{
    void handle_input()
    {
        Input& input = Input::instance();
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

std::function<void()>& GameLoop::get()
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
    LevelGenerator::instance().getLevel().initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_UPPER);
    LevelGenerator::instance().getLevel().generate_cave_background();
    LevelGenerator::instance().getLevel().batch_vertices();
    LevelGenerator::instance().getLevel().add_render_entity();

    _loop = [this]()
    {
        auto& camera = Camera::instance();
        auto& level_renderer = Renderer::instance();

        camera.update_gl_modelview_matrix();

        level_renderer.render();
        level_renderer.update();

        handle_input();
    };
}
