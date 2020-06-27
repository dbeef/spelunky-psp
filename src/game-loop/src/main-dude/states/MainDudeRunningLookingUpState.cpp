#include "main-dude/states/MainDudeRunningLookingUpState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeRunningLookingUpState::enter(MainDude &main_dude)
{
    main_dude._physics.set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);

    if (main_dude._states.current == &main_dude._states.running)
    {
        main_dude._animation.resume(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUNNING_LOOKING_UP_0_FIRST),
                                    static_cast<std::size_t>(MainDudeSpritesheetFrames::RUNNING_LOOKING_UP_5_LAST));
    }
    else
    {
        main_dude._animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUNNING_LOOKING_UP_0_FIRST),
                                   static_cast<std::size_t>(MainDudeSpritesheetFrames::RUNNING_LOOKING_UP_5_LAST),
                                   75, true);
    }
}

MainDudeBaseState *MainDudeRunningLookingUpState::update(MainDude& main_dude, uint32_t delta_time_ms)
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

MainDudeBaseState *MainDudeRunningLookingUpState::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.left())
    {
        main_dude._physics.add_velocity(-MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    if (input.right())
    {
        main_dude._physics.add_velocity(MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    if (input.circle())
    {
        main_dude._physics.add_velocity(0.0f, -MainDude::JUMP_SPEED);
        return &main_dude._states.jumping;
    }
    if (input.cross())
    {
        return &main_dude._states.crawling;
    }

    if (input.bumper_l())
    {
        main_dude._physics.set_max_x_velocity(MainDude::MAX_RUNNING_VELOCITY_X);
        main_dude._animation.set_time_per_frame_ms(50);
    }
    else
    {
        main_dude._physics.set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);
        main_dude._animation.set_time_per_frame_ms(75);
    }

    if (input.bumper_r())
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
        return this;
    }

    return &main_dude._states.running;
}
