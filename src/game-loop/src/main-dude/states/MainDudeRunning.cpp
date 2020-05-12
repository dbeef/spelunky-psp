#include <main-dude/MainDude.hpp>
#include "main-dude/states/MainDudeRunning.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

#include <cstring>

void MainDudeRunning::enter(MainDude &main_dude)
{

}

MainDudeBaseState *MainDudeRunning::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._input_component.update(main_dude, delta_time_ms);
    main_dude._physics_component.update(main_dude, delta_time_ms);
    main_dude._quad_component.update(main_dude, delta_time_ms);

    // Other:

    if (main_dude._physics_component.get_x_velocity() == 0.0f)
    {
        return &main_dude._states.standing;
    }
    else
    {
        main_dude._facing_left = main_dude._physics_component.get_x_velocity() < 0.0f;
    }

    main_dude._animation_update_delta_ms += delta_time_ms;

    const auto current_frame = main_dude._quad_component.get_current_frame<MainDudeSpritesheetFrames>();

    const bool running_animation = current_frame >= MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST &&
                             current_frame <= MainDudeSpritesheetFrames::RUN_LEFT_5_LAST;
    const bool is_last_frame = current_frame == MainDudeSpritesheetFrames::RUN_LEFT_5_LAST;

    if (!running_animation)
    {
        main_dude._quad_component.frame_changed(MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST);
    }
    else if (main_dude._animation_update_delta_ms >= 75)
    {
        if (is_last_frame)
        {
            main_dude._quad_component.frame_changed(MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST);
        }
        else
        {
            auto next_frame = static_cast<MainDudeSpritesheetFrames>( static_cast<int>(current_frame ) + 1);
            main_dude._quad_component.frame_changed(next_frame);
        }
        main_dude._animation_update_delta_ms = 0;
    }

    return this;
}
