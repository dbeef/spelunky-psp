#include "video/Video.hpp"
#include "graphics_utils/DebugGlCall.hpp"
#include "time/Timestep.hpp"
#include "logger/log.h"
#include "glad/glad.h"
#include "video/ScopedImguiContext.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>

struct PlatformSpecific
{
    SDL_Window* window;
    void* gl_context;
    std::unique_ptr<ScopedImguiContext> imgui;
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
    _platform_specific->imgui = nullptr;
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

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 6 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_LoadLibrary(nullptr);
    SDL_ClearError();

    //  Create a window

    _platform_specific->window = SDL_CreateWindow("Spelunky",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              480 * 2, 272 * 2,
                              SDL_WINDOW_OPENGL |
                              SDL_WINDOW_ALLOW_HIGHDPI);
    if (!_platform_specific->window)
    {
        log_error("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

	int w, h;
	SDL_GL_GetDrawableSize(_platform_specific->window, &w, &h);
    _viewport = std::make_shared<Viewport>(w, h);

    _platform_specific->gl_context = SDL_GL_CreateContext(_platform_specific->window);
    if (!_platform_specific->gl_context)
    {
        log_error("SDL_GL_CreateContext Error: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    SDL_GL_MakeCurrent(_platform_specific->window, _platform_specific->gl_context);
    SDL_ClearError();

    if(!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        log_error("Error while loading ptrs to OpenGL functions");
        return false;
    }

    DebugGlCall(glEnable(GL_TEXTURE_2D));

    DebugGlCall(glShadeModel(GL_SMOOTH));
    DebugGlCall(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));

    DebugGlCall(glEnable(GL_BLEND));
    DebugGlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    DebugGlCall(glEnableClientState(GL_VERTEX_ARRAY)); // For glVertexPointer
    DebugGlCall(glEnableClientState(GL_TEXTURE_COORD_ARRAY)); // For glTexCoordPointer

    // Disable, as it is not used and may affect performance:
    DebugGlCall(glDisableClientState(GL_COLOR_ARRAY));
    DebugGlCall(glDisableClientState(GL_NORMAL_ARRAY));

    DebugGlCall(glDisable(GL_FOG));
    DebugGlCall(glDisable(GL_LIGHTING));
    DebugGlCall(glDisable(GL_CULL_FACE));
    DebugGlCall(glDisable(GL_ALPHA_TEST));
    DebugGlCall(glDisable(GL_COLOR_LOGIC_OP));
    DebugGlCall(glDisable(GL_DITHER));
    DebugGlCall(glDisable(GL_STENCIL_TEST));
    DebugGlCall(glDisable(GL_DEPTH_TEST));
    DebugGlCall(glDisable(GL_POINT_SMOOTH));
    DebugGlCall(glDisable(GL_LINE_SMOOTH));
    DebugGlCall(glDisable(GL_SCISSOR_TEST));
    DebugGlCall(glDisable(GL_COLOR_MATERIAL));
    DebugGlCall(glDisable(GL_NORMALIZE));
    DebugGlCall(glDisable(GL_RESCALE_NORMAL));

    log_info("Initializing imgui");
    _platform_specific->imgui = std::make_unique<ScopedImguiContext>(_platform_specific->window, _platform_specific->gl_context);

    log_info("Exiting Video::setup_gl, success.");
    return true;
}

