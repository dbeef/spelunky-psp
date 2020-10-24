//
// Created by dbeef on 7/7/19.
//
#include "video/Context.hpp"

#include "glad/glad.h"
#include "graphics_utils/DebugGlCall.hpp"
#include "logger/log.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>

#include <fstream>
#include <string>

bool Video::setup_gl()
{
    log_info("Entered Video::setup_gl");

    // http://sdl.beuc.net/sdl.wiki/SDL_envvars
    // https://wiki.dingoonity.org/index.php?title=Dingux:OpenDingux:Development
    // https://www.kernel.org/doc/html/latest/fb/fbcon.html
    // https://wiki.libsdl.org/FAQUsingSDL#How_do_I_choose_a_specific_video_driver.3F
    setenv("DISPLAY=:0") ;
    setenv("SDL_VIDEODRIVER=directfb") ;
    setenv("SDL_VIDEO_GL_DRIVER=libGLESv1_CM.so.1") ;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        log_error("SDL_Init Error: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 1 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
    
    //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 1 );
    //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    //SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    
    //SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    //SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    //SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

    SDL_GL_LoadLibrary("libGLESv1_CM.so.1");
    SDL_ClearError();

    //  Create a window
    
    window = SDL_CreateWindow("Spelunky", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    0,
                                    0,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (!window)
    {
        log_info("Failed to create window, reason: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    _viewport = std::make_shared<Viewport>(w, h);

    log_info("Window size: %i %i", w, h);

    glContext = SDL_GL_CreateContext(window);

    if (!glContext)
    {
        log_info("Failed to create glContext, reason: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }
    
    SDL_GL_MakeCurrent(window, glContext);
    SDL_ClearError();

    log_info("hiding cursor...");
    SDL_ShowCursor(0);

    log_info("Calling glad loader...");
    
    if(!gladLoadGLES1Loader((GLADloadproc) SDL_GL_GetProcAddress)) {
        log_error("Error while loading ptrs to OpenGL functions");
        return false;
    }

    log_info("Glad OK, now OpenGL setup...");
    SDL_ClearError();

    DebugGlCall(glEnable(GL_TEXTURE_2D));

    DebugGlCall(glShadeModel(GL_SMOOTH));
    DebugGlCall(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));

    DebugGlCall(glEnable(GL_BLEND));
    DebugGlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    DebugGlCall(glEnableClientState(GL_VERTEX_ARRAY)); // For glVertexPointer
    DebugGlCall(glEnableClientState(GL_TEXTURE_COORD_ARRAY)); // For glTexCoordPointer

    log_info("Exiting Video::setupGL, success.");
    return true;
}
