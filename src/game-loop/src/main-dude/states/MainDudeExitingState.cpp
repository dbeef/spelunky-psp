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
    main_dude._quad.update(main_dude.get_x_pos_center(), main_dude.get_y_pos_center(), !main_dude._other.facing_left);

    if (main_dude._animation.is_finished())
    {
        main_dude._other.entered_door = true;
    }

    return this;
}

MainDudeBaseState *MainDudeExitingState::handle_input(MainDude& main_dude, const Input &input)
{
    return this;
}
