#pragma once

#include <cstdint>

#include "TextureType.hpp"
#include "Quad.hpp"
#include "MapTile.hpp"

class MainDude;

class AnimationComponent
{
public:

    void update(MainDude&, uint32_t delta_time_ms);
    void start(std::size_t start_frame, std::size_t end_frame, uint32_t time_per_frame_ms, bool loop);
    void stop() { _running = false; }
    bool is_finished() const { return _finished; }

private:

    bool _running = false;
    bool _finished = false;
    bool _loop = false;

    float _timer_ms = 0;
    float _time_per_frame_ms = 0;

    std::size_t _current_frame_index = 0;
    std::size_t _start_frame_index = 0;
    std::size_t _end_frame_index = 0;
};
