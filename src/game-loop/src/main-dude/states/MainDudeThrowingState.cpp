#include "main-dude/states/MainDudeThrowingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeThrowingState::enter(MainDude &main_dude)
{
    main_dude._animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::THROWING_LEFT_0_FIRST),
                               static_cast<std::size_t>(MainDudeSpritesheetFrames::THROWING_LEFT_8_LAST),
                               50, false);
}

MainDudeBaseState *MainDudeThrowingState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude.get_x_pos_center(), main_dude.get_y_pos_center(), !main_dude._other.facing_left);
    main_dude._animation.update(main_dude, delta_time_ms);

    // Other:

    if (main_dude._animation.is_finished())
    {
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
        else
        {
            if (main_dude._physics.get_y_velocity() > 0.0f)
            {
                return &main_dude._states.falling;
            }
            else if (main_dude._physics.get_y_velocity() < 0.0f)
            {
                return &main_dude._states.jumping;
            }
        }
    }

    return this;
}

MainDudeBaseState *MainDudeThrowingState::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.left())
    {
        main_dude._physics.add_velocity(-MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    if (input.right())
    {
        main_dude._physics.add_velocity(MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    if (input.jumping() && main_dude._physics.is_bottom_collision())
    {
        main_dude._physics.add_velocity(0.0f, -MainDude::JUMP_SPEED);
    }

    if (input.running_fast())
    {
        main_dude._physics.set_max_x_velocity(MainDude::MAX_RUNNING_VELOCITY_X);
    }
    else
    {
        main_dude._physics.set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);
    }

    return this;
}
