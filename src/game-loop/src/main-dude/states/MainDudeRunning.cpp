#include <cstring>
#include <main-dude/MainDude.hpp>
#include "main-dude/states/MainDudeRunning.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

MainDudeRunning::MainDudeRunning()
{

}

MainDudeBaseState *MainDudeRunning::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    Input& input = Input::instance();

    // TODO: Bring back input component
    if (input.square())
    {
        main_dude._physics_component.add_velocity(-0.025f, 0.0f);
    }
    if (input.circle())
    {
        main_dude._physics_component.add_velocity(+0.025f, 0.0f);
    }
    if (input.triangle())
    {
        if (main_dude._physics_component.is_bottom_collision())
        {
            main_dude._physics_component.add_velocity(0.0f, -0.18f);
        }
    }

    main_dude._physics_component.update(main_dude, delta_time_ms);
    main_dude._quad_component.update(main_dude, delta_time_ms);
    main_dude._animation_update_delta_ms += delta_time_ms;

    if (main_dude._physics_component.get_x_velocity() == 0.0f)
    {
        return &main_dude._states.standing;
    }
    else
    {
        main_dude._facing_left = main_dude._physics_component.get_x_velocity() < 0.0f;
    }

    const auto current_frame = main_dude._quad_component.get_current_frame<MainDudeSpritesheetFrames>();
    
    bool running_animation_on = current_frame >= MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST &&
                                current_frame <= MainDudeSpritesheetFrames::RUN_LEFT_5_LAST;
    bool last_frame = current_frame == MainDudeSpritesheetFrames::RUN_LEFT_5_LAST;

    if (!running_animation_on || last_frame)
    {
        main_dude._quad_component.frame_changed(MainDudeSpritesheetFrames ::RUN_LEFT_0_FIRST);
    }
    else
    {
        if (main_dude._animation_update_delta_ms >= 75)
        {
            auto next_frame = static_cast<MainDudeSpritesheetFrames>( static_cast<int>(current_frame ) + 1);
            main_dude._quad_component.frame_changed(next_frame);

            main_dude._animation_update_delta_ms = 0;
        }
    }
    return this;
}

void MainDudeRunning::enter(MainDude &main_dude)
{

}
