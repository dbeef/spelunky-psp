#include "main-dude/states/MainDudeFallingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeFallingState::enter(MainDude& main_dude)
{
    main_dude._physics.set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);
    main_dude._animation.stop();
    main_dude._quad.frame_changed(MainDudeSpritesheetFrames::JUMP_LEFT);
}

MainDudeBaseState* MainDudeFallingState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude, delta_time_ms);
    main_dude._camera.update(main_dude, delta_time_ms);

    // Other:

    if (main_dude._physics.is_bottom_collision())
    {
        if (main_dude._physics.get_x_velocity() == 0.0f)
        {
            return &main_dude._states.standing;
        }
        else
        {
            return &main_dude._states.running;
        }
    }

    return this;
}

MainDudeBaseState *MainDudeFallingState::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.square())
    {
        main_dude._physics.add_velocity(-MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    if (input.circle())
    {
        main_dude._physics.add_velocity(MainDude::DEFAULT_DELTA_X, 0.0f);
    }

    if (input.bumper_l())
    {
        main_dude._physics.set_max_x_velocity(MainDude::MAX_RUNNING_VELOCITY_X);
        main_dude._animation.set_time_per_frame_ms(50);
    }
    else
    {
        main_dude._physics.set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);
        main_dude._animation.set_time_per_frame_ms(75);
    }

    if (input.bumper_r())
    {
        return &main_dude._states.throwing;
    }

    if (input.bumper_l()) // FIXME: Awkward key mapping, change once camera following is implemented
    {
        const auto* exit_tile = main_dude.is_overlaping_exit();
        if (exit_tile)
        {
            main_dude._physics.set_position(
                    exit_tile->x + main_dude._quad.get_quad_width() / 2,
                    exit_tile->y + main_dude._quad.get_quad_height() / 2);

            return &main_dude._states.exiting;
        }
    }

    return this;
}
