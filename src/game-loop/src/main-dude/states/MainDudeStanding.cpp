#include "main-dude/states/MainDudeStanding.hpp"
#include "main-dude/MainDude.hpp"

void MainDudeStanding::enter(MainDude& main_dude)
{
    main_dude._animation.stop();
    main_dude._quad.frame_changed(MainDudeSpritesheetFrames::STAND_LEFT);
}

MainDudeBaseState *MainDudeStanding::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._input.update(main_dude, delta_time_ms);
    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude, delta_time_ms);

    // Other:

    const auto velocity_x = main_dude._physics.get_x_velocity();
    if (velocity_x != 0.0f)
    {
        return &main_dude._states.running;
    }
    else
    {
        return this;
    }
}
