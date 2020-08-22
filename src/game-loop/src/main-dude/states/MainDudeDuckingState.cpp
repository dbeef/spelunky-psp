#include "main-dude/states/MainDudeDuckingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeDuckingState::enter(MainDude& main_dude)
{
    main_dude._animation.stop();
    main_dude._quad.frame_changed(MainDudeSpritesheetFrames::CRAWLING_LEFT_0_FIRST);
}

MainDudeBaseState* MainDudeDuckingState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(delta_time_ms);
    main_dude._quad.update(main_dude.get_x_pos_center(), main_dude.get_y_pos_center(), !main_dude._other.facing_left);

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

MainDudeBaseState *MainDudeDuckingState::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.left().value())
    {
        main_dude._physics.add_velocity(-MainDude::DEFAULT_DELTA_X, 0.0f);
        return &main_dude._states.crawling;
    }
    if (input.right().value())
    {
        main_dude._physics.add_velocity(MainDude::DEFAULT_DELTA_X, 0.0f);
        return &main_dude._states.crawling;
    }
    if (input.jumping().changed() && input.jumping().value())
    {
        main_dude._physics.add_velocity(0.0f, -MainDude::JUMP_SPEED);
        return &main_dude._states.jumping;
    }
    if (!input.ducking().value())
    {
        return &main_dude._states.standing;
    }

    if (input.throwing().changed() && input.throwing().value())
    {
        return &main_dude._states.throwing;
    }

    if (input.up().value())
    {
        const auto* exit_tile = main_dude.is_overlaping_tile(MapTileType::EXIT);
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
