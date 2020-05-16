#include <main-dude/MainDude.hpp>
#include "main-dude/states/MainDudeDuckingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeDucking::enter(MainDude& main_dude)
{
    main_dude._animation.stop();
    main_dude._quad.frame_changed(MainDudeSpritesheetFrames::CRAWLING_LEFT_0_FIRST);
}

MainDudeBaseState* MainDudeDucking::update(MainDude& main_dude, uint32_t delta_time_ms)
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

    return this;
}

MainDudeBaseState *MainDudeDucking::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.square())
    {
        main_dude._physics.add_velocity(-0.045f, 0.0f);
        return &main_dude._states.crawling;
    }
    if (input.circle())
    {
        main_dude._physics.add_velocity(+0.045f, 0.0f);
        return &main_dude._states.crawling;
    }
    if (input.triangle())
    {
        main_dude._physics.add_velocity(0.0f, -0.18f);
        return &main_dude._states.jumping;
    }
    if (!input.cross())
    {
        return &main_dude._states.standing;
    }

    return this;
}
