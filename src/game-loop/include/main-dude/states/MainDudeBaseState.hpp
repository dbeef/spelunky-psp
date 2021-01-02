#pragma once

#include <cstdint>
#include <entt/entt.hpp>

class Input;
class MainDudeComponent;

class MainDudeBaseState
{
public:
    ~MainDudeBaseState() = default;

    virtual MainDudeBaseState* update(MainDudeComponent&, uint32_t delta_time_ms) = 0;

    virtual void enter(MainDudeComponent&) = 0;
    virtual void exit(MainDudeComponent&) {}; // Default definition provided as most states don't utilize the exit function.
};
