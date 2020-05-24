#include "main-dude/states/MainDudeStandingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeStandingState::enter(MainDude& main_dude)
{
    _x_collision_timer = 0;
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

    if (main_dude._physics.get_y_velocity() > 0.0f)
    {
        return &main_dude._states.falling;
    }
    else if (main_dude._physics.get_y_velocity() < 0.0f)
    {
        return &main_dude._states.jumping;
    }

    if (main_dude._physics.is_left_collision() || main_dude._physics.is_right_collision())
    {
        _x_collision_timer += delta_time_ms;
        if (_x_collision_timer > 400)
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

MainDudeBaseState *MainDudeStandingState::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.left())
    {
        main_dude._physics.add_velocity(-MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    if (input.right())
    {
        main_dude._physics.add_velocity(MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    if (input.circle())
    {
        main_dude._physics.add_velocity(0.0f, -MainDude::JUMP_SPEED);
        return &main_dude._states.jumping;
    }
    if (input.cross())
    {
        return &main_dude._states.ducking;
    }
    if (input.bumper_r())
    {
        return &main_dude._states.throwing;
    }

    if (input.up())
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
