#include "main-dude/states/MainDudeCrawlingState.hpp"
#include "main-dude/MainDude.hpp"

void MainDudeCrawling::enter(MainDude& main_dude)
{
}

MainDudeBaseState* MainDudeCrawling::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // TODO
    return this;
}

MainDudeBaseState *MainDudeCrawling::handle_input(MainDude &, const Input &input)
{
    return this;
}
