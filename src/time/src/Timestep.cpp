#include <SDL/SDL_timer.h>
#include "time/Timestep.hpp"

void Timestep::delay() const
{
    const auto delta = std::chrono::duration_cast<std::chrono::microseconds>(_end - _start);

    if (delta > _frequency)
    {
        // no delay
    } else
    {
        const auto sleep_time = _frequency - delta;
        SDL_Delay(sleep_time.count() / 1000);
    }
}
