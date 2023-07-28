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

class Singletons final
{
public:
  Singletons()
  {
    Level::init();
    TextureBank::init();
    Input::init();
    Audio::init();
    Video::init();
    Inventory::init();
    EntityRegistry::init();
  }
  ~Singletons()
  {
    EntityRegistry::dispose();
    Inventory::dispose();
    Video::dispose();
    Audio::dispose();
    Input::dispose();
    TextureBank::dispose();
    Level::dispose();
  }
};

#if defined(SPELUNKY_PSP_PLATFORM_EMSCRIPTEN)
#include <emscripten.h>
void run_loop(void* cb_data)
{
  auto& video = Video::instance();
  auto* game_loop = reinterpret_cast<GameLoop*>(cb_data);
  video.tick(game_loop->get());
}
#endif

int start()
{
    log_info("Started.");
    Singletons raii_singletons;

    if (!Audio::instance().setup_audio())
    {
        log_error("Failed to setup audio.");
        return EXIT_FAILURE;
    }

    if (!Video::instance().setup_gl())
    {
        log_error("Failed to setup OpenGL.");
        return EXIT_FAILURE;
    }

    {
        auto& video = Video::instance();
        GameLoop loop(video.get_viewport());

        #if defined(SPELUNKY_PSP_PLATFORM_EMSCRIPTEN)
        emscripten_set_main_loop_arg(run_loop, (void*)&loop, 60, true);
        #else
        while (!video.tick(loop.get())) { }
        #endif
    }

    Video::instance().tear_down_gl();
    Audio::instance().tear_down_audio();

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
    return start();
}

#endif
