// TODO: Disable adding this signature in IDE. Add a line in coding guideline concerning this.
//
// Created by dbeef on 7/7/19.
//

#include <SDL/SDL.h>
#include <iostream>
#include <Input.hpp>
#include "video/Context.hpp"

#include "time/Timestep.hpp"
#include "glad/glad.h"

bool Video::setupGL() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        SDL_ClearError();
        return false;
    }

    //  Create a window
    auto surface = SDL_SetVideoMode(getWindowWidth(),
                                    getWindowHeight(),
                                    0, // current display's bpp
                                    SDL_DOUBLEBUF | SDL_OPENGL | SDL_SWSURFACE);

    if (!surface) {
        std::cout << "SDL_SetVideoMode Error: " << SDL_GetError() << std::endl;
        SDL_ClearError();
        return false;
    }

    if(!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        printf("Error while loading ptrs to OpenGL functions\n");
        return false;
    }

    // TODO: OpenGL asserts.
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}
