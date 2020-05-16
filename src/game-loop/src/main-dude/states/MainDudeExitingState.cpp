#include <main-dude/MainDude.hpp>
#include "main-dude/states/MainDudeExitingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeExitingState::enter(MainDude& main_dude)
{
    main_dude._animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::EXITING_LEFT_0_FIRST),
                               static_cast<std::size_t>(MainDudeSpritesheetFrames::EXITING_LEFT_15_LAST),
                               75, false);
}

MainDudeBaseState* MainDudeExitingState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    main_dude._animation.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude, delta_time_ms);

    if (main_dude._animation.is_finished())
    {
        // Normally should enter next level, yet for now just change state:
        return &main_dude._states.standing;
    }

    return this;
}

MainDudeBaseState *MainDudeExitingState::handle_input(MainDude& main_dude, const Input &input)
{
    return this;
}
