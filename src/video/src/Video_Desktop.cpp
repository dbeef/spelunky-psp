#include "video/Video.hpp"
#include "graphics_utils/DebugGlCall.hpp"
#include "time/Timestep.hpp"
#include "logger/log.h"
#include "glad/glad.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <stdexcept>

#if defined(SPELUNKY_PSP_WITH_IMGUI)
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl2.h"

namespace {
    class Imgui {
    public:
        Imgui(SDL_Window *window, void *gl_context) {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            (void) io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            // Setup Platform/Renderer backends
            if (!ImGui_ImplSDL2_InitForOpenGL(window, gl_context) ||
                !ImGui_ImplOpenGL2_Init()) {
                throw std::runtime_error("Failed to initialize imgui with SDL2 + OpenGL");
            }
        }

        ~Imgui() {
            ImGui_ImplOpenGL2_Shutdown();
            ImGui_ImplSDL2_Shutdown();
            ImGui::DestroyContext();
        }
    };
}
#else
struct Imgui { Imgui(SDL_Window *window, void *gl_context) {}};
#endif

namespace
{
    std::pair<int, int> query_screen_dimensions() {
        SDL_DisplayMode dm;
        if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
        {
            throw std::runtime_error(std::string("SDL_GetDesktopDisplayMode failed: ") + SDL_GetError());
        }
        return {dm.w, dm.h};
    }
}

struct PlatformSpecific
{
    SDL_Window* window;
    void* gl_context;
    std::unique_ptr<Imgui> imgui;
};

Video::~Video() = default; // For pimpl idiom.
Video::Video() : _timestep(60) {}; // For pimpl idiom.

void Video::tear_down_gl()
{
    SDL_GL_DeleteContext(_platform_specific->gl_context);
    SDL_DestroyWindow(_platform_specific->window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
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

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0) {
        log_error("SDL_Init Error: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 6 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GL_LoadLibrary(nullptr);
    SDL_ClearError();

    //  Create a window
    const auto screen_dimensions = query_screen_dimensions();
    const auto& screen_h = screen_dimensions.second;
    const auto scale_coeff = 0.8f;

    _platform_specific->window = SDL_CreateWindow("SpelunkyPSP",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              screen_h * scale_coeff * (16.0f / 9), screen_h * scale_coeff,
                              SDL_WINDOW_OPENGL |
                              SDL_WINDOW_ALLOW_HIGHDPI);
    if (!_platform_specific->window)
    {
        log_error("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    int window_w, window_h;
    SDL_GL_GetDrawableSize(_platform_specific->window, &window_w, &window_h);
    _viewport = std::make_shared<Viewport>(window_w, window_h);

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

    log_info("Initializing imgui");
    _platform_specific->imgui = std::make_unique<Imgui>(_platform_specific->window, _platform_specific->gl_context);

    log_info("Exiting Video::setup_gl, success.");
    return true;
}
