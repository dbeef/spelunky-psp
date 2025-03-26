#pragma once

#include "main-dude/states/MainDudeBaseState.hpp"

class MainDudeThrowingState : public MainDudeBaseState
{
public:

    MainDudeBaseState* update(MainDudeComponent&, uint32_t delta_time_ms) override;
    void enter(MainDudeComponent&) override;

private:
    uint32_t _x_collision_timer = 0;
};
