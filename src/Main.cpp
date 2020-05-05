#include "video/Context.hpp"
#include "logger/log.h"
#include "Camera.hpp"
#include "LevelGenerator.hpp"
#include "LevelRenderer.hpp"
#include "Input.hpp"
#include "OAMRegistry.hpp"
#include "GameLoop.hpp"
#include "TextureBank.hpp"

#include <cstdlib>

void init_singletons()
{
    Camera::init();
    LevelGenerator::init();
    LevelRenderer::init();
    OAMRegistry::init();
    TextureBank::init();
    Input::init();
    Video::init();
}

void dispose_singletons()
{
    Video::dispose();
    Camera::dispose();
    LevelGenerator::dispose();
    TextureBank::dispose();
    OAMRegistry::dispose();
    LevelRenderer::dispose();
}

int start()
{
    log_info("Started.");
    init_singletons();

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

    TextureBank::instance().load_textures();
    TextureBank::instance().load_texture_regions();

    LevelRenderer::instance().batch_vertices();

    {
        GameLoop loop;
        Video::instance().run_loop(loop.get());
    }

    Video::instance().tear_down_gl();

    dispose_singletons();
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
