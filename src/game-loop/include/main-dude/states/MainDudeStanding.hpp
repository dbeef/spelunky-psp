#pragma once

#include "main-dude/states/MainDudeBaseState.hpp"

class MainDudeStanding : public MainDudeBaseState
{
public:

    MainDudeStanding();
    MainDudeBaseState* update(MainDude& main_dude, uint32_t delta_time_ms) override;
    void enter(MainDude&) override;
};
