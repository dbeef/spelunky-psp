#pragma once

#include "main-dude/states/MainDudeBaseState.hpp"

class MainDudeStanding : public MainDudeBaseState
{
public:

    MainDudeBaseState* update(MainDude& main_dude, uint32_t delta_time_ms) override;
    MainDudeBaseState* handle_input(MainDude&, const Input& input) override;
    void enter(MainDude&) override;

private:
    uint32_t _x_collision_timer = 0;
};
