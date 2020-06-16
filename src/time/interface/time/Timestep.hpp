#pragma once

#include <SDL/SDL_timer.h>
#include <thread>

using Timestamp = Uint32;
using Frequency = Uint32;

class Timestep {

public:

    explicit Timestep(float fps = 60) : _fps(fps), _frequency(1000.0f * (1.0f / fps))
    {
    }

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
    const Frequency _frequency;
    const float _fps;
};
