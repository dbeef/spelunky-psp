#include "main-dude/states/MainDudeRunning.hpp"
#include "main-dude/MainDude.hpp"

void MainDudeRunning::enter(MainDude &main_dude)
{
    main_dude._animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST),
                               static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_5_LAST),
                               75, true);
}

MainDudeBaseState *MainDudeRunning::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._input.update(main_dude, delta_time_ms);
    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude, delta_time_ms);
    main_dude._animation.update(main_dude, delta_time_ms);

    // Other:
    
    if (main_dude._physics.get_x_velocity() == 0.0f)
    {
        return &main_dude._states.standing;
    }

    return this;
}
