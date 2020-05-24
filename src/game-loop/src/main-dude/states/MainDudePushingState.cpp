#include "main-dude/states/MainDudePushingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudePushingState::enter(MainDude& main_dude)
{
    main_dude._animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::PUSH_LEFT_0_FIRST),
                               static_cast<std::size_t>(MainDudeSpritesheetFrames::PUSH_LEFT_6_LAST),
                               75, true);
}

MainDudeBaseState* MainDudePushingState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude, delta_time_ms);
    main_dude._animation.update(main_dude, delta_time_ms);

    // Other:

    if (!main_dude._physics.is_left_collision() && !main_dude._physics.is_right_collision())
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

MainDudeBaseState *MainDudePushingState::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.left())
    {
        main_dude._physics.add_velocity(-MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    else if (main_dude._other.facing_left)
    {
        return &main_dude._states.running;
    }

    if (input.right())
    {
        main_dude._physics.add_velocity(MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    else if (!main_dude._other.facing_left)
    {
        return &main_dude._states.running;
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

    return this;
}
