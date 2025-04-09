#pragma once

#include <cstdint>
#include "main-dude/states/MainDudeBaseState.hpp"

class MainDudeStunnedState : public MainDudeBaseState
{
public:

    MainDudeBaseState* update(MainDudeComponent&, uint32_t delta_time_ms) override;

    void enter(MainDudeComponent&) override;
    void exit(MainDudeComponent&) override;
private:
    uint16_t _stunned_timer_ms = 0;
};
