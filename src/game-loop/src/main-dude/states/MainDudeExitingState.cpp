#include "main-dude/states/MainDudeExitingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeExitingState::enter(MainDude& main_dude)
{
}

MainDudeBaseState* MainDudeExitingState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // TODO
    return this;
}

MainDudeBaseState *MainDudeExitingState::handle_input(MainDude& main_dude, const Input &input)
{
    return this;
}
