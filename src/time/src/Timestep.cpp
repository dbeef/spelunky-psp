#include "time/Timestep.hpp"

void Timestep::delay() const
{
    const auto delta_ms = (_end - _start);

    if (delta_ms > _frequency)
    {
        // no delay
    }
    else
    {
        auto sleep_time_ms = _frequency - delta_ms;
        SDL_Delay(sleep_time_ms);
    }
}
