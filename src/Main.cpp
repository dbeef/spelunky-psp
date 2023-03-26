#include <emscripten.h>
#include "video/Video.hpp"
#include "audio/Audio.hpp"
#include "logger/log.h"
#include "Level.hpp"
#include "Input.hpp"
#include "game-loop/GameLoop.hpp"
#include "other/Inventory.hpp"
#include "TextureBank.hpp"
#include "EntityRegistry.hpp"

#include <cstdlib>

void init_singletons()
{
    Level::init();
    TextureBank::init();
    Input::init();
    Audio::init();
    Inventory::init();
    EntityRegistry::init();
}

void dispose_singletons()
{
    EntityRegistry::dispose();
    Inventory::dispose();
    Audio::dispose();
    Input::dispose();
    TextureBank::dispose();
    Level::dispose();
}

void run_loop(void* cb_data)
{
    auto* video = reinterpret_cast<Video*>(cb_data);
    // while (true)
    {
        // log_info("Tick");
        video->loop_tick();
    }
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
        log_info("Creating GameLoop instance");
        GameLoop loop(video.get_viewport());
        log_info("Running the loop");
        video.run_loop(loop.get());
        emscripten_set_main_loop_arg(run_loop, (void*)&video, 60, true);
    }

    video.tear_down_gl();
    Audio::instance().tear_down_audio();

    dispose_singletons();
    log_info("Exiting peacefully.");
    return EXIT_SUCCESS;
}

#if defined(SPELUNKY_PSP_PLATFORM_PSP) || \
    defined(SPELUNKY_PSP_PLATFORM_ANDROID) || \
    defined(SPELUNKY_PSP_PLATFORM_EMSCRIPTEN)
// Not mangling symbols so SDL could find SDL_main.
extern "C"
{
int SDL_main(int argc, char *argv[]) {
    return start();
}
}
#endif

#if defined(SPELUNKY_PSP_PLATFORM_LINUX) || \
    defined(SPELUNKY_PSP_PLATFORM_WINDOWS) || \
    defined(SPELUNKY_PSP_PLATFORM_EMSCRIPTEN) || \
    defined(SPELUNKY_PSP_PLATFORM_DARWIN)

int main()
{
    // EM_ASM({ Module.wasmTable = wasmTable; });
    return start();
}

#endif
