#include "Input.hpp"

#include <SDL_events.h>
#include <SDL_keyboard.h>

#include <cstdlib>

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

const char* Input::get_scores_request_binding_msg()
{
    return "SPACE";
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

    SDL_Event event{};

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EventType::SDL_KEYDOWN || event.type == SDL_EventType::SDL_KEYUP)
        {
            const auto& key = event.key.keysym.sym;
            const bool v = event.type == SDL_EventType::SDL_KEYDOWN;

            // FIXME: Weird keybindings due to no letter key events issued on Android's SDL2.
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
            else if (key == SDLK_SPACE)
            {
                _toggles.jumping.feed(v);
            }
            else if (key == SDLK_PAGEUP)
            {
                _toggles.ducking.feed(v);
            }
            else if (key == SDLK_LSHIFT)
            {
                _toggles.running_fast.feed(v);
            }
            else if (key == SDLK_TAB)
            {
                _toggles.throwing.feed(v);
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
            else if (key == SDLK_HOME)
            {
                _toggles.out_bomb.feed(v);
            }
            else if (key == SDLK_END)
            {
                _toggles.out_rope.feed(v);
            }
        }

        fill_input_events();
    }
}
