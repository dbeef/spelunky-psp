#pragma once

#include "main-dude/states/MainDudeBaseState.hpp"

class MainDudeLevelSummaryState : public MainDudeBaseState
{
public:

    MainDudeBaseState* update(MainDude&, uint32_t delta_time_ms) override;
    MainDudeBaseState* handle_input(MainDude&, const Input& input) override;
    void enter(MainDude&) override;
};
