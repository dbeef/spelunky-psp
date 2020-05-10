#include "Input.hpp"

#include <SDL/SDL_events.h>

void Input::poll()
{
    SDL_Event event{};

    while (SDL_PollEvent(&event))
    {
        _exit = event.type == SDL_QUIT;

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
        }
    }
}
