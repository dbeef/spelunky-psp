#include "main-dude/states/MainDudeStanding.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

#include <cstring>

void MainDudeStanding::enter(MainDude& main_dude)
{
    main_dude._animation_component.stop();
    main_dude._quad_component.frame_changed(MainDudeSpritesheetFrames::STAND_LEFT);
}

MainDudeBaseState *MainDudeStanding::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._input_component.update(main_dude, delta_time_ms);
    main_dude._physics_component.update(main_dude, delta_time_ms);
    main_dude._quad_component.update(main_dude, delta_time_ms);

    // Other:

    const auto velocity_x = main_dude._physics_component.get_x_velocity();
    if (velocity_x != 0.0f)
    {
        return &main_dude._states.running;
    }
    else
    {
        return this;
    }
}
