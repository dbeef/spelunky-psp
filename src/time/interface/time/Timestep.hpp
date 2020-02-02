#pragma once

#include <chrono>
#include <thread>

using Timestamp = std::chrono::system_clock::time_point;

class Timestep {

public:

    explicit Timestep(double fps = 60) : _fps(fps), _frequency(std::chrono::microseconds(static_cast<unsigned long long>(1000 * 1000 * (1 / fps))))
    {
    }

    inline void mark_start()
    {
        _start = std::chrono::system_clock::now();
    }

    inline void mark_end()
    {
        _end = std::chrono::system_clock::now();
    }

    void delay() const;

    inline std::uint64_t get_delta() const { return std::chrono::duration_cast<std::chrono::microseconds>(_end - _start).count();  };

private:

    Timestamp _start;
    Timestamp _end;
    const std::chrono::microseconds _frequency;
    const double _fps;
};
