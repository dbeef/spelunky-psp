#include "time/Timestep.hpp"

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
