#pragma once

#include <SDL/SDL_timer.h>
#include <thread>

using Timestamp = Uint32;
using Timediff = Uint32;

class Timestep {

public:

    explicit Timestep(float fps = 60.f)
        : _fps(fps)
        , _min_delay_ms(static_cast<Timediff>(1000.0f / fps))
    { }

    inline void mark_start()
    {
        _start = SDL_GetTicks();
    }

    inline void mark_end()
    {
        _end = SDL_GetTicks();
    }

    Timestamp delay() const;

private:

    Timestamp _start;
    Timestamp _end;
    const float _fps;
    const Timediff _min_delay_ms;
};
