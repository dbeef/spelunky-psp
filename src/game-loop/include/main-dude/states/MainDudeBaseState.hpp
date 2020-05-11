#pragma once

#include <cstdint>

class MainDude;
class MainDudeBaseState
{
public:
    ~MainDudeBaseState() = default;
    virtual MainDudeBaseState* update(MainDude&, uint32_t delta_time_ms) = 0;
    virtual void enter(MainDude&) = 0;
};
