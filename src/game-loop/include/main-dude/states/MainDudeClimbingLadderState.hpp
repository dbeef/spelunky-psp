#pragma once

#include "main-dude/states/MainDudeBaseState.hpp"

class MainDudeClimbingLadderState : public MainDudeBaseState
{
public:

    MainDudeBaseState* update(MainDude&, uint32_t delta_time_ms) override;
    MainDudeBaseState* handle_input(MainDude&, const Input& input) override;
    void enter(MainDude&) override;
    void exit(MainDude&) override;
private:
    uint32_t _climbing_ladder_timer = 0;
    bool _climbing_ladder_sound_toggle = false;
    bool _climbing_ladder_sound_playing = false;

    void play_sound();
};
