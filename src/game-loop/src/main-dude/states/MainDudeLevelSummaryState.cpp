#include "main-dude/states/MainDudeLevelSummaryState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeLevelSummaryState::enter(MainDude& main_dude)
{
    main_dude._physics.set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);
    main_dude._animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST),
                               static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_5_LAST),
                               75, true);
}

MainDudeBaseState* MainDudeLevelSummaryState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude, delta_time_ms);
    main_dude._animation.update(main_dude, delta_time_ms);

    // Other:
    main_dude._physics.set_velocity(0.095f, 0.0f);

    const auto* exit_tile = main_dude.is_overlaping_tile(MapTileType::EXIT);
    if (exit_tile)
    {
        main_dude._physics.set_position(
                exit_tile->x + main_dude._quad.get_quad_width() / 2,
                exit_tile->y + main_dude._quad.get_quad_height() / 2);

        return &main_dude._states.exiting;
    }

    return this;
}

MainDudeBaseState *MainDudeLevelSummaryState::handle_input(MainDude& main_dude, const Input &input)
{
    // Input is blocked in this state.
    return this;
}

