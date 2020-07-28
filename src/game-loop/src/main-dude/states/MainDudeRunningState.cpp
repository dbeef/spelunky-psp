#include "main-dude/states/MainDudeRunningState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeRunningState::enter(MainDude &main_dude)
{
    main_dude._physics.set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);
    if (main_dude._states.current == &main_dude._states.running_looking_up)
    {
        main_dude._animation.resume(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST),
                                    static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_5_LAST));
    }
    else
    {
        main_dude._animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST),
                                   static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_5_LAST),
                                   75, true);
    }
}

MainDudeBaseState *MainDudeRunningState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude.get_x_pos_center(), main_dude.get_y_pos_center(), !main_dude._other.facing_left);
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
    if (input.left().value())
    {
        main_dude._physics.add_velocity(-MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    if (input.right().value())
    {
        main_dude._physics.add_velocity(MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    if (input.jumping().changed() && input.jumping().value())
    {
        main_dude._physics.add_velocity(0.0f, -MainDude::JUMP_SPEED);
        return &main_dude._states.jumping;
    }
    if (input.ducking().value())
    {
        return &main_dude._states.crawling;
    }

    if (input.running_fast().value())
    {
        main_dude._physics.set_max_x_velocity(MainDude::MAX_RUNNING_VELOCITY_X);
        main_dude._animation.set_time_per_frame_ms(50);
    }
    else
    {
        main_dude._physics.set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);
        main_dude._animation.set_time_per_frame_ms(75);
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

        return &main_dude._states.running_looking_up;
    }

    return this;
}
