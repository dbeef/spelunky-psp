#include <main-dude/MainDude.hpp>
#include <Collisions.hpp>
#include <LevelGenerator.hpp>
#include <logger/log.h>
#include "main-dude/states/MainDudeJumpingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeJumpingState::enter(MainDude& main_dude)
{
    main_dude._physics.set_max_y_velocity(MainDude::DEFAULT_MAX_Y_VELOCITY);
    main_dude._physics.set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);
    main_dude._animation.stop();
    main_dude._quad.frame_changed(MainDudeSpritesheetFrames::JUMP_LEFT);
}

MainDudeBaseState* MainDudeJumpingState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude.get_x_pos_center(), main_dude.get_y_pos_center(), !main_dude._other.facing_left);

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

MainDudeBaseState *MainDudeJumpingState::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.left())
    {
        main_dude._physics.add_velocity(-MainDude::DEFAULT_DELTA_X, 0.0f);
        if (main_dude.hang_off_cliff_left())
        {
            return &main_dude._states.cliff_hanging;
        }
    }
    if (input.right())
    {
        main_dude._physics.add_velocity(MainDude::DEFAULT_DELTA_X, 0.0f);
        if (main_dude.hang_off_cliff_right())
        {
            return &main_dude._states.cliff_hanging;
        }
    }

    if (input.running_fast())
    {
        main_dude._physics.set_max_x_velocity(MainDude::MAX_RUNNING_VELOCITY_X);
        main_dude._animation.set_time_per_frame_ms(50);
    }
    else
    {
        main_dude._physics.set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);
        main_dude._animation.set_time_per_frame_ms(75);
    }

    if (input.throwing())
    {
        return &main_dude._states.throwing;
    }

    if (input.up())
    {
        const auto* exit_tile = main_dude.is_overlaping_tile(MapTileType::EXIT);
        if (exit_tile)
        {
            main_dude._physics.set_position(
                    exit_tile->x + main_dude._quad.get_quad_width() / 2,
                    exit_tile->y + main_dude._quad.get_quad_height() / 2);

            return &main_dude._states.exiting;
        }

        const auto* ladder_tile = main_dude.is_overlaping_tile(MapTileType::LADDER);
        const auto* ladder_deck_tile = main_dude.is_overlaping_tile(MapTileType::LADDER_DECK);

        if (ladder_tile || ladder_deck_tile)
        {
            const auto* tile = ladder_tile ? ladder_tile : ladder_deck_tile;
            main_dude._physics.set_position(
                    tile->x + main_dude._quad.get_quad_width() / 2,
                    main_dude.get_y_pos_center());

            return &main_dude._states.climbing;
        }
    }

    return this;
}
