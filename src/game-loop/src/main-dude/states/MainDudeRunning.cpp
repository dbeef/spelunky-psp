#include <main-dude/MainDude.hpp>
#include "main-dude/states/MainDudeRunning.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

#include <cstring>

void MainDudeRunning::enter(MainDude &main_dude)
{
    main_dude._animation_component.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST),
                                         static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_5_LAST),
                                         75, true);
}

MainDudeBaseState *MainDudeRunning::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._input_component.update(main_dude, delta_time_ms);
    main_dude._physics_component.update(main_dude, delta_time_ms);
    main_dude._quad_component.update(main_dude, delta_time_ms);
    main_dude._animation_component.update(main_dude, delta_time_ms);

    // Other:

    if (main_dude._physics_component.get_x_velocity() == 0.0f)
    {
        return &main_dude._states.standing;
    }
    else
    {
        main_dude._facing_left = main_dude._physics_component.get_x_velocity() < 0.0f;
    }

    return this;
}
