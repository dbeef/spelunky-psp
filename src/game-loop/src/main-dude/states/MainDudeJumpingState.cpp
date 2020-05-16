#include "main-dude/states/MainDudeJumpingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeJumping::enter(MainDude& main_dude)
{
    main_dude._animation.stop();
    main_dude._quad.frame_changed(MainDudeSpritesheetFrames::JUMP_LEFT);
}

MainDudeBaseState* MainDudeJumping::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude, delta_time_ms);

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

MainDudeBaseState *MainDudeJumping::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.square())
    {
        main_dude._physics.add_velocity(-0.025f, 0.0f);
    }
    if (input.circle())
    {
        main_dude._physics.add_velocity(+0.025f, 0.0f);
    }

    return this;
}
