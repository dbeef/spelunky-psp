//
// Created by dbeef on 7/7/19.
//

#pragma once

#include "time/Timestep.hpp"
#include "viewport/Viewport.hpp"

#include <functional>
#include <memory>
#include <cassert>

struct PlatformSpecific;

class Video {

public:

    Video();
    ~Video();

    bool setup_gl();
    inline uint32_t get_delta_time() const { return _last_delta_time; }
    void tear_down_gl();
    void run_loop(const std::function<bool(uint32_t delta_time_ms)> &loop_callback);
    std::shared_ptr<Viewport> get_viewport() const { assert(_viewport); return _viewport; }
    void swap_buffers() const;

private:
    Timestep _timestep;

    std::unique_ptr<PlatformSpecific> _platform_specific;
    std::shared_ptr<Viewport> _viewport {nullptr};
    uint32_t _last_delta_time = 0;
};
