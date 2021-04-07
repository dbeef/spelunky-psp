#pragma once

#include "main-dude/states/MainDudeBaseState.hpp"

class MainDudeClimbingState : public MainDudeBaseState
{
public:

    MainDudeBaseState* update(MainDudeComponent&, uint32_t delta_time_ms) override;

    void enter(MainDudeComponent&) override;
    void exit(MainDudeComponent&) override;
private:
    uint32_t _climbing_ladder_timer = 0;
    bool _climbing_ladder_sound_toggle = false;
    bool _climbing_ladder_sound_playing = false;

    void play_sound();
};
