#include "Input.hpp"

#include <SDL/SDL_events.h>
#include <cstdlib>

void Input::poll()
{
    SDL_Event event{};

    while (SDL_PollEvent(&event))
    {
        _paused = event.type == SDL_QUIT || (event.type == SDL_EventType::SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE);
        if (_paused)
        {
            // FIXME: Better exit handling
            std::exit(0);
        }

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
        }
    }
}
