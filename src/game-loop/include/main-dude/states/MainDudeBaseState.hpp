#pragma once

#include <cstdint>

class Input;
class MainDude;

class MainDudeBaseState
{
public:
    ~MainDudeBaseState() = default;

    virtual MainDudeBaseState* update(MainDude&, uint32_t delta_time_ms) = 0;
    virtual MainDudeBaseState* handle_input(MainDude&, const Input& input) = 0;
    virtual void enter(MainDude&) = 0;
};
