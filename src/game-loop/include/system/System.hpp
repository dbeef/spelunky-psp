#pragma once

#include <entt/entt.hpp>
#include <cstdint>

class System
{
public:
    virtual void update(std::uint32_t delta_time_ms) = 0;
};
