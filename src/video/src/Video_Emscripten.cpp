#include "video/Video.hpp"
#include "graphics_utils/DebugGlCall.hpp"
#include "time/Timestep.hpp"
#include "logger/log.h"

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_opengl.h>

struct PlatformSpecific
{
    SDL_Window* window;
    void* gl_context;
};

Video::~Video() = default; // For pimpl idiom.
Video::Video() : _timestep(60) {}; // For pimpl idiom.

void Video::tear_down_gl()
{
    SDL_GL_DeleteContext(_platform_specific->gl_context);
    SDL_DestroyWindow(_platform_specific->window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    _platform_specific->window = nullptr;
    _platform_specific->gl_context = nullptr;
}

void Video::swap_buffers() const
{
    SDL_GL_SwapWindow(_platform_specific->window);
}

bool Video::setup_gl()
{
    log_info("Entered Video::setup_gl.");

    _platform_specific = std::make_unique<PlatformSpecific>();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_error("SDL_Init Error: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    log_info("Setting SDL_GL attributes");

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 6 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

    SDL_ClearError();

    //  Create a window

    log_info("Creating a window");

    _platform_specific->window = SDL_CreateWindow("Spelunky",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              1600,900, // FIXME: Query the web browser for canvas dimensions
                              SDL_WINDOW_OPENGL);

    if (!_platform_specific->window)
    {
        log_error("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    log_info("Querying for window size");

	int w, h;
	SDL_GetWindowSize(_platform_specific->window, &w, &h);
    _viewport = std::make_shared<Viewport>(w, h);

    log_info("Creating GL context");

    _platform_specific->gl_context = SDL_GL_CreateContext(_platform_specific->window);
    if (!_platform_specific->gl_context)
    {
        log_error("SDL_GL_CreateContext Error: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    log_info("Setting OpenGL properties");

    DebugGlCall(glEnable(GL_TEXTURE_2D));
    DebugGlCall(glShadeModel(GL_SMOOTH));
    DebugGlCall(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));
    DebugGlCall(glEnable(GL_BLEND));
    DebugGlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    DebugGlCall(glEnableClientState(GL_VERTEX_ARRAY)); // For glVertexPointer
    DebugGlCall(glEnableClientState(GL_TEXTURE_COORD_ARRAY)); // For glTexCoordPointer

    log_info("Exiting Video::setup_gl, success.");
    return true;
}

