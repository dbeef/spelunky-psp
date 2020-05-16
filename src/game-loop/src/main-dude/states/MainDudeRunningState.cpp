#include <main-dude/MainDude.hpp>
#include "main-dude/states/MainDudeRunningState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeRunningState::enter(MainDude &main_dude)
{
    main_dude._animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST),
                               static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_5_LAST),
                               75, true);
}

MainDudeBaseState *MainDudeRunningState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude, delta_time_ms);
    main_dude._animation.update(main_dude, delta_time_ms);

    // Other:
    
    if (main_dude._physics.get_x_velocity() == 0.0f)
    {
        return &main_dude._states.standing;
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

MainDudeBaseState *MainDudeRunningState::handle_input(MainDude& main_dude, const Input &input)
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
        main_dude._physics.add_velocity(0.0f, -0.18f);
        return &main_dude._states.jumping;
    }
    if (input.cross())
    {
        return &main_dude._states.crawling;
    }

    return this;
}
