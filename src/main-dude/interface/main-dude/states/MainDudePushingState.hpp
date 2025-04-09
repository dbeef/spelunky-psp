#pragma once

#include "main-dude/states/MainDudeBaseState.hpp"

class MainDudePushingState : public MainDudeBaseState
{
public:

    MainDudeBaseState* update(MainDudeComponent&, uint32_t delta_time_ms) override;
    void enter(MainDudeComponent&) override;
};
