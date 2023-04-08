//
// Created by dbeef on 7/7/19.
//

#pragma once

#include "patterns/Singleton.hpp"
#include "time/Timestep.hpp"
#include "viewport/Viewport.hpp"

#include <functional>
#include <memory>
#include <cassert>

struct PlatformSpecific;

class Video  : public Singleton<Video>
{
public:

    using TickCallback = std::function<bool(uint32_t delta_time_ms)>;

    ~Video();

    DELETE_COPY_MOVE_CONSTRUCTORS(Video)
    FRIEND_SINGLETON(Video)

    bool setup_gl();
    void tear_down_gl();
    std::shared_ptr<Viewport> get_viewport() const { assert(_viewport); return _viewport; }
    void swap_buffers() const;
    bool tick(const TickCallback&);
private:
    Video();
    Timestep _timestep;
    std::unique_ptr<PlatformSpecific> _platform_specific;
    std::shared_ptr<Viewport> _viewport {nullptr};
    uint32_t _last_delta_ms = 0;
};
