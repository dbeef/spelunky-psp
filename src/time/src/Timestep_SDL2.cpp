#include "time/Timestep.hpp"

// FIXME: Forcing SDL2; move all platforms to SDL2 (Windows left)
#include <SDL2/SDL_timer.h>

Timestamp Timestep::delay() const
{
    const auto delta_ms = (_end - _start);

    if (delta_ms < _min_delay_ms)
    {
        const auto sleep_time_ms = _min_delay_ms - delta_ms;
        SDL_Delay(sleep_time_ms);
        return _min_delay_ms;
    }
    return delta_ms;
}

void Timestep::mark_start()
{
    _start = SDL_GetTicks();
}

void Timestep::mark_end()
{
    _end = SDL_GetTicks();
}
