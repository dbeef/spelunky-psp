#include "video/Context.hpp"
#include "logger/log.h"
#include "Camera.hpp"
#include "LevelGenerator.hpp"
#include "LevelRenderer.hpp"
#include "Input.hpp"
#include "graphics_utils/DebugGlCall.hpp"
#include "game_loop/interface/GameLoop.hpp"

#include <cstdlib>

int start()
{
    log_info("Started.");

    Camera::init();
    LevelGenerator::init();
    LevelRenderer::init();
    Input::init();
    Video::init();

    if (!Video::instance().setup_gl())
    {
        log_error("Failed to setup OpenGL.");
        return EXIT_FAILURE;
    }

    Camera::instance().update_gl_projection_matrix();

    // TODO: State pattern: "main menu" state should handle this, or "start game" because of also loading assets
    LevelGenerator::instance().getLevel().clean_map_layout();
    LevelGenerator::instance().getLevel().generate_frame();
    LevelGenerator::instance().getLevel().initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU_UPPER);

    LevelRenderer::instance().load_textures();
    LevelRenderer::instance().batch_vertices();

    {
        GameLoop loop;
        Video::instance().run_loop(loop.get());
    }

    Video::instance().tear_down_gl();
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
