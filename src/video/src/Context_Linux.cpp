// TODO: Disable adding this signature in IDE. Add a line in coding guideline concerning this.
//
// Created by dbeef on 7/7/19.
//

#include <SDL/SDL.h>

#include "graphics_utils/DebugGlCall.hpp"
#include "video/Context.hpp"
#include "glad/glad.h"
#include "logger/log.h"

bool Video::setupGL() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_error("SDL_Init Error: %s");
        SDL_ClearError();
        return false;
    }

    //  Create a window
    auto surface = SDL_SetVideoMode(getWindowWidth(),
                                    getWindowHeight(),
                                    0, // current display's bpp
                                    SDL_DOUBLEBUF | SDL_OPENGL | SDL_SWSURFACE);

    if (!surface) {
        log_error("SDL_SetVideoMode Error: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    if(!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        log_error("Error while loading ptrs to OpenGL functions");
        return false;
    }

    DebugGlCall(glEnable(GL_TEXTURE_2D));
    DebugGlCall(glShadeModel(GL_SMOOTH));
    DebugGlCall(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));
    DebugGlCall(glEnable(GL_BLEND));
    DebugGlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    log_info("Exiting Video::setupGL, success.");
    return true;
}
