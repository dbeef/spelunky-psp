#pragma once

#include "main-dude/states/MainDudeBaseState.hpp"

class MainDudeDeadState : public MainDudeBaseState
{
public:

    MainDudeBaseState* update(MainDude&, World* world, uint32_t delta_time_ms) override;
    MainDudeBaseState* handle_input(MainDude&, World* world, const Input& input) override;
    void enter(MainDude&) override;
    void exit(MainDude&) override;
private:
    void set_current_frame(MainDude&);
};
