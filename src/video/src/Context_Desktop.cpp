//
// Created by dbeef on 7/7/19.
//

#include <SDL/SDL.h>

#include "graphics_utils/DebugGlCall.hpp"
#include "video/Context.hpp"
#include "glad/glad.h"
#include "logger/log.h"

bool Video::setup_gl()
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        log_error("SDL_Init Error: %s");
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
    auto surface = SDL_SetVideoMode(0,
                                    0,
                                    0, // current display's bpp
                                    SDL_DOUBLEBUF | SDL_OPENGL | SDL_SWSURFACE | SDL_FULLSCREEN);



    if (!surface)
    {
        log_error("SDL_SetVideoMode Error: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
    {
        log_error("Error while loading ptrs to OpenGL functions");
        return false;
    }

    _width = surface->w;
    _height = surface->h;
    _aspect = static_cast<float>(_width) / _height;

    DebugGlCall(glEnable(GL_TEXTURE_2D));
    DebugGlCall(glShadeModel(GL_SMOOTH));
    DebugGlCall(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));

    DebugGlCall(glEnable(GL_BLEND));
    DebugGlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    DebugGlCall(glEnableClientState(GL_VERTEX_ARRAY)); // For glVertexPointer
    DebugGlCall(glEnableClientState(GL_TEXTURE_COORD_ARRAY)); // For glTexCoordPointer

    // Disable, as it is not used and may affect performance:
    DebugGlCall(glDisableClientState(GL_COLOR_ARRAY));
    DebugGlCall(glDisableClientState(GL_EDGE_FLAG_ARRAY));
    DebugGlCall(glDisableClientState(GL_NORMAL_ARRAY));
    DebugGlCall(glDisableClientState(GL_INDEX_ARRAY));

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

    log_info("Exiting Video::setupGL, success.");
    return true;
}
