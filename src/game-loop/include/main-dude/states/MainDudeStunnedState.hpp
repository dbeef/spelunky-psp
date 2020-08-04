#pragma once

#include <cstdint>
#include "main-dude/states/MainDudeBaseState.hpp"

class MainDudeStunnedState : public MainDudeBaseState
{
public:

    MainDudeBaseState* update(MainDude& main_dude, uint32_t delta_time_ms) override;
    MainDudeBaseState* handle_input(MainDude&, const Input& input) override;
    void enter(MainDude&) override;
private:
    uint16_t _stunned_timer_ms = 0;
};
