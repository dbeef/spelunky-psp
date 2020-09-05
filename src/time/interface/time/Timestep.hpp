#pragma once

#include <cstdint>
#include <thread>

using Timestamp = uint32_t;
using Timediff = uint32_t;

class Timestep {

public:

    explicit Timestep(float fps = 60.f) : _min_delay_ms(static_cast<Timediff>(1000.0f / fps))
    { }

    void mark_start();

    void mark_end();

    Timestamp delay() const;

private:

    Timestamp _start = 0;
    Timestamp _end = 0;
    const Timediff _min_delay_ms;
};
