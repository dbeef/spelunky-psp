//
// Created by dbeef on 7/7/19.
//
#include "video/Video.hpp"
#include "SDL_opengl.h"
#include "graphics_utils/DebugGlCall.hpp"
#include "logger/log.h"

#include <SDL/SDL.h>

struct PlatformSpecific
{
    // Nothing platform specific to be stored.
};

Video::~Video() = default; // For pimpl idiom.
Video::Video() : _timestep(60) {}; // For pimpl idiom.

void Video::tear_down_gl()
{
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Video::swap_buffers() const
{
    SDL_GL_SwapBuffers();
}

bool Video::setup_gl()
{
    log_info("Entered Video::setup_gl.");

    _platform_specific = std::make_unique<PlatformSpecific>();
    _viewport = std::make_shared<Viewport>(480, 272);

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        log_error("SDL_Init Error: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 0);
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0);

    SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 0);
    SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 0);
    SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 0);
    SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 0);

    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    //  Create a window
    auto surface = SDL_SetVideoMode(_viewport->get_width_pixels(),
                                    _viewport->get_height_pixels(),
                                    0, // current display's bpp
                                    SDL_DOUBLEBUF | SDL_OPENGL | SDL_SWSURFACE);

    if (!surface) {
        log_error("SDL_SetVideoMode Error: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    if(!gladLoadGLES1Loader((GLADloadproc) SDL_GL_GetProcAddress)) {
        log_error("Error while loading ptrs to OpenGL functions");
        return false;
    }

    SDL_ClearError();

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

    log_info("Exiting Video::setup_gl, success.");
    return true;
}
