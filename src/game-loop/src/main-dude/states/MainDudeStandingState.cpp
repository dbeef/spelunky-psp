#include "main-dude/states/MainDudeStandingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeStandingState::enter(MainDude& main_dude)
{
    main_dude._animation.stop();
    main_dude._quad.frame_changed(MainDudeSpritesheetFrames::STAND_LEFT);
}

MainDudeBaseState *MainDudeStandingState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude, delta_time_ms);

    // Other:

    if (main_dude._physics.get_x_velocity() != 0.0f)
    {
        return &main_dude._states.running;
    }
    else
    {
        if (main_dude._physics.is_left_collision() || main_dude._physics.is_right_collision())
        {
            _x_collision_timer += delta_time_ms;
            if (_x_collision_timer > 2000)
            {
                _x_collision_timer = 0;
                return &main_dude._states.pushing;
            }
        }
        else
        {
            _x_collision_timer = 0;
        }

        return this;
    }
}

MainDudeBaseState *MainDudeStandingState::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.square())
    {
        main_dude._physics.add_velocity(-0.025f, 0.0f);
    }
    if (input.circle())
    {
        main_dude._physics.add_velocity(+0.025f, 0.0f);
    }
    if (input.triangle())
    {
        if (main_dude._physics.is_bottom_collision())
        {
            main_dude._physics.add_velocity(0.0f, -0.18f);
            return &main_dude._states.jumping;
        }
    }

    return this;
}
