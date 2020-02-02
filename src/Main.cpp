#include "video/Context.hpp"
#include "logger/log.h"
#include "Camera.hpp"
#include "LevelGenerator.hpp"
#include "LevelRenderer.hpp"
#include "Input.hpp"
#include "graphics_utils/DebugGlCall.hpp"
#include "glad/glad.h"

#include <cstdlib>

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

    std::function<void()> create_game_loop()
    {
        return []()
        {
            DebugGlCall(glClear(GL_COLOR_BUFFER_BIT));
            LevelRenderer::instance().render();
            handle_input();
        };
    }
}

int start()
{
    log_info("Started.");

    Camera::init();
    LevelGenerator::init();
    LevelRenderer::init();
    Input::init();

    LevelGenerator::instance().getLevel().clean_map_layout();
    LevelGenerator::instance().getLevel().generate_frame();
    LevelGenerator::instance().getLevel().initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_UPPER);

    Video::init();

    if (!Video::instance().setupGL())
    {
        log_error("Failed to setup OpenGL.");
        return EXIT_FAILURE;
    }

    LevelRenderer::instance().load_textures();
    LevelRenderer::instance().set_projection_matrix();

    auto loop = create_game_loop();
    Video::instance().runLoop(loop);
    Video::instance().tearDownGL();
    Video::dispose();

    Camera::dispose();
    LevelGenerator::dispose();
    LevelRenderer::dispose();

    log_info("Exiting peacefully.");
    return EXIT_SUCCESS;
}

#if defined(SPELUNKY_PSP_PLATFORM_PSP)

// Not mangling symbols so SDL could find SDL_main.
extern "C"
{
int SDL_main(int argc, char *argv[]) {
    return start();
}
}
#endif

#if defined(SPELUNKY_PSP_PLATFORM_LINUX)

int main()
{
    return start();
}

#endif
