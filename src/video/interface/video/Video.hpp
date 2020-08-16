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
    inline uint32_t get_delta_time() const { return _last_delta_time; }
    void tear_down_gl();
    void run_loop(const std::function<bool(uint32_t delta_time_ms)> &loop_callback);
    std::shared_ptr<Viewport> get_viewport() const { assert(_viewport); return _viewport; }
    inline void swap_buffers() const;

private:
    Timestep _timestep;

    std::shared_ptr<Viewport> _viewport;
    uint32_t _last_delta_time = 0;

};
