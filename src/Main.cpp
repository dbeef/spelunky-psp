#include "video/Context.hpp"
#include "logger/log.h"
#include "LevelGenerator.hpp"
#include "Renderer.hpp"
#include "Input.hpp"
#include "GameLoop.hpp"
#include "TextureBank.hpp"

#include <cstdlib>

void init_singletons()
{
    LevelGenerator::init();
    Renderer::init();
    TextureBank::init();
    Input::init();
    Video::init();
}

void dispose_singletons()
{
    Video::dispose();
    LevelGenerator::dispose();
    TextureBank::dispose();
    Renderer::dispose();
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

#if defined(SPELUNKY_PSP_PLATFORM_LINUX) || defined(SPELUNKY_PSP_PLATFORM_WINDOWS)

int main()
{
    return start();
}

#endif
