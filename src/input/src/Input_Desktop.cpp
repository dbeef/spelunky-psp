#include "Input.hpp"

#include <SDL/SDL_events.h>
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

void Input::poll()
{
    SDL_Event event{};

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EventType::SDL_KEYDOWN || event.type == SDL_EventType::SDL_KEYUP)
        {
            const SDLKey& key = event.key.keysym.sym;
            const bool v = event.type == SDL_EventType::SDL_KEYDOWN;

            if (key == SDLK_LEFT)
            {
                _left = v;
            }
            else if (key == SDLK_RIGHT)
            {
                _right = v;
            }
            else if (key == SDLK_UP)
            {
                _up = v;
            }
            else if (key == SDLK_DOWN)
            {
                _down = v;
            }
            else if (key == SDLK_d)
            {
                _jumping = v;
            }
            else if (key == SDLK_s)
            {
                _ducking = v;
            }
            else if (key == SDLK_LSHIFT)
            {
                _running_fast = v;
            }
            else if (key == SDLK_q)
            {
                _throwing = v;
            }
            else if (key == SDLK_ESCAPE)
            {
                _paused = v;
            }
            else if (key == SDLK_F1)
            {
                _death_requested = v;
            }
            else if (key == SDLK_F10)
            {
                _quit_requested = v;
            }
        }
    }
}
