#include "video/Video.hpp"
#include "audio/Audio.hpp"
#include "logger/log.h"
#include "Level.hpp"
#include "Renderer.hpp"
#include "Input.hpp"
#include "GameLoop.hpp"
#include "TextureBank.hpp"

#include <cstdlib>

void init_singletons()
{
    Renderer::init();
    Level::init();
    TextureBank::init();
    Input::init();
    Audio::init();
}

void dispose_singletons()
{
    Audio::dispose();
    Input::dispose();
    TextureBank::dispose();
    Level::dispose();
    Renderer::dispose();
}

int start()
{
    log_info("Started.");
    init_singletons();

    if (!Audio::instance().setup_audio())
    {
        log_error("Failed to setup audio.");
        return EXIT_FAILURE;
    }

    Video video;

    if (!video.setup_gl())
    {
        log_error("Failed to setup OpenGL.");
        return EXIT_FAILURE;
    }

    {
        GameLoop loop(video.get_viewport());
        video.run_loop(loop.get());
    }

    video.tear_down_gl();
    Audio::instance().tear_down_audio();

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
