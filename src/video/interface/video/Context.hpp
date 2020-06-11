//
// Created by dbeef on 7/7/19.
//

#pragma once

#include "time/Timestep.hpp"
#include "viewport/Viewport.hpp"

#include <functional>
#include <memory>
#include <cassert>

class Video {

public:

    Video() : _timestep(60) {};

    bool setup_gl();

    void tear_down_gl();

    void run_loop(const std::function<void(uint32_t delta_time_ms)> &loop_callback);

    std::shared_ptr<Viewport> get_viewport() const { assert(_viewport); return _viewport; }

    inline void swap_buffers() const;

private:
    Timestep _timestep;

    std::shared_ptr<Viewport> _viewport;

};
