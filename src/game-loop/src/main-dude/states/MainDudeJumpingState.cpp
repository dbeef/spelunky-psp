#include "main-dude/states/MainDudeJumpingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"
#include "audio/Audio.hpp"

void MainDudeJumpingState::enter(MainDude& main_dude)
{
    Audio::instance().play(SFXType::MAIN_DUDE_JUMP);

    auto* physics = main_dude.get_physics_component();
    auto* animation = main_dude.get_animation_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(animation);
    assert(quad);

    physics->set_max_y_velocity(MainDude::DEFAULT_MAX_Y_VELOCITY);
    physics->set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);
    animation->stop();
    quad->frame_changed(MainDudeSpritesheetFrames::JUMP_LEFT);
}

MainDudeBaseState* MainDudeJumpingState::update(MainDude& main_dude, World* world, uint32_t delta_time_ms)
{
    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(quad);

    physics->update(world, delta_time_ms);
    quad->update(physics->get_x_position(), physics->get_y_position(), !main_dude._other.facing_left);

    if (physics->is_bottom_collision())
    {
        if (physics->get_x_velocity() == 0.0f)
        {
            return &main_dude._states.standing;
        }
        else
        {
            return &main_dude._states.running;
        }
    }
    else if (physics->get_y_velocity() > 0.0f)
    {
        return &main_dude._states.falling;
    }

    return this;
}

MainDudeBaseState *MainDudeJumpingState::handle_input(MainDude& main_dude, World* world, const Input &input)
{
    auto* physics = main_dude.get_physics_component();
    auto* animation = main_dude.get_animation_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(animation);
    assert(quad);

    if (input.left().value())
    {
        physics->add_velocity(-MainDude::DEFAULT_DELTA_X, 0.0f);
        if (main_dude.hang_off_cliff_left(world))
        {
            return &main_dude._states.cliff_hanging;
        }
    }
    if (input.right().value())
    {
        physics->add_velocity(MainDude::DEFAULT_DELTA_X, 0.0f);
        if (main_dude.hang_off_cliff_right(world))
        {
            return &main_dude._states.cliff_hanging;
        }
    }

    if (input.running_fast().value())
    {
        physics->set_max_x_velocity(MainDude::MAX_RUNNING_VELOCITY_X);
        animation->set_time_per_frame_ms(50);
    }
    else
    {
        physics->set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);
        animation->set_time_per_frame_ms(75);
    }

    if (input.throwing().changed() && input.throwing().value())
    {
        return &main_dude._states.throwing;
    }

    if (input.up().value())
    {
        const auto* exit_tile = main_dude.is_overlaping_tile(world, MapTileType::EXIT);
        if (exit_tile)
        {
            physics->set_position(
                    exit_tile->x + quad->get_quad_width() / 2,
                    exit_tile->y + quad->get_quad_height() / 2);

            return &main_dude._states.exiting;
        }

        const auto* ladder_tile = main_dude.is_overlaping_tile(world, MapTileType::LADDER);
        const auto* ladder_deck_tile = main_dude.is_overlaping_tile(world, MapTileType::LADDER_DECK);

        if (ladder_tile || ladder_deck_tile)
        {
            const auto* tile = ladder_tile ? ladder_tile : ladder_deck_tile;
            physics->set_position(
                    tile->x + quad->get_quad_width() / 2,
                    physics->get_y_position());

            return &main_dude._states.climbing;
        }
    }

    return this;
}
