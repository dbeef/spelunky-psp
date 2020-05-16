#include <main-dude/MainDude.hpp>
#include "main-dude/states/MainDudeCrawlingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeCrawling::enter(MainDude& main_dude)
{
    main_dude._physics.set_max_x_velocity(MainDude::MAX_CRAWLING_VELOCITY_X);
    main_dude._physics.set_friction(MainDude::CRAWLING_FRICTION_COEFFICIENT);
    main_dude._animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::CRAWLING_LEFT_0_FIRST),
                               static_cast<std::size_t>(MainDudeSpritesheetFrames::CRAWLING_LEFT_9_LAST),
                               75, true);
}

MainDudeBaseState* MainDudeCrawling::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude, delta_time_ms);
    main_dude._animation.update(main_dude, delta_time_ms);

    // Other:

    if (main_dude._physics.get_x_velocity() == 0.0f)
    {
        return &main_dude._states.ducking;
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

MainDudeBaseState *MainDudeCrawling::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.square())
    {
        main_dude._physics.add_velocity(-MainDude::CRAWLING_DELTA_X, 0.0f);
    }
    if (input.circle())
    {
        main_dude._physics.add_velocity(MainDude::CRAWLING_DELTA_X, 0.0f);
    }
    if (input.triangle())
    {
        main_dude._physics.add_velocity(0.0f, -MainDude::JUMP_SPEED);
        return &main_dude._states.jumping;
    }
    if (!input.cross())
    {
        return &main_dude._states.running;
    }

    return this;
}
