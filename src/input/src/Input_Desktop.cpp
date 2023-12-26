#include "Input.hpp"

#include <SDL2/SDL_events.h>

#if defined(SPELUNKY_PSP_WITH_IMGUI)
    #include "imgui_impl_sdl2.h"
    void imgui_event_processing(const SDL_Event* event) { ImGui_ImplSDL2_ProcessEvent(event); }
#else
    void imgui_event_processing(const SDL_Event* event) {}
#endif


const char* Input::get_pause_binding_msg()
{
    return "ESC";
}

const char* Input::get_death_request_binding_msg()
{
    return "F1";
}

const char* Input::get_quit_request_binding_msg()
{
    return "F10";
}

const char* Input::get_accept_transaction_binding_msg()
{
    return "P";
}

const char* Input::get_scores_request_binding_msg()
{
    return "P";
}

void Input::poll()
{
    // As this input implementation relies on key events, to not end up with 'changed' being true until
    // the next event, reset the property on every input poll:
    _toggles.paused.reset_changed();
    _toggles.death_requested.reset_changed();
    _toggles.quit_requested.reset_changed();
    _toggles.left.reset_changed();
    _toggles.right.reset_changed();
    _toggles.up.reset_changed();
    _toggles.down.reset_changed();
    _toggles.jumping.reset_changed();
    _toggles.ducking.reset_changed();
    _toggles.running_fast.reset_changed();
    _toggles.throwing.reset_changed();
    _toggles.out_bomb.reset_changed();
    _toggles.out_rope.reset_changed();
    _toggles.purchase.reset_changed();
    _toggles.cheat_console.reset_changed();

    SDL_Event event{};

    while (SDL_PollEvent(&event))
    {
        imgui_event_processing(&event);
        if (event.type == SDL_EventType::SDL_KEYDOWN || event.type == SDL_EventType::SDL_KEYUP)
        {
            const auto& key = event.key.keysym.sym;
            const bool v = event.type == SDL_EventType::SDL_KEYDOWN;
            if (key == SDLK_LEFT)
            {
                _toggles.left.feed(v);
            }
            else if (key == SDLK_RIGHT)
            {
                _toggles.right.feed(v);
            }
            else if (key == SDLK_UP)
            {
                _toggles.up.feed(v);
            }
            else if (key == SDLK_DOWN)
            {
                _toggles.down.feed(v);
            }
            else if (key == SDLK_d)
            {
                _toggles.jumping.feed(v);
            }
            else if (key == SDLK_s)
            {
                _toggles.ducking.feed(v);
            }
            else if (key == SDLK_LSHIFT)
            {
                _toggles.running_fast.feed(v);
            }
            else if (key == SDLK_q)
            {
                _toggles.throwing.feed(v);
            }
            else if (key == SDLK_z)
            {
                _toggles.out_bomb.feed(v);
            }
            else if (key == SDLK_p)
            {
                _toggles.purchase.feed(v);
            }
            else if (key == SDLK_x)
            {
                _toggles.out_rope.feed(v);
            }
            else if (key == SDLK_ESCAPE)
            {
                _toggles.paused.feed(v);
            }
            else if (key == SDLK_F1)
            {
                _toggles.death_requested.feed(v);
            }
            else if (key == SDLK_F10)
            {
                _toggles.quit_requested.feed(v);
            }
            else if (key == SDLK_TAB)
            {
                _toggles.cheat_console.feed(v);
            }
        }
    }

    fill_input_events();
}
