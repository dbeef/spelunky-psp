#include "main-dude/states/MainDudeStandingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"
#include "logger/log.h"

void MainDudeStandingState::enter(MainDude& main_dude)
{
    auto* animation = main_dude.get_animation_component();
    auto* quad = main_dude.get_quad_component();

    assert(animation);
    assert(quad);

    animation->stop();
    quad->frame_changed(MainDudeSpritesheetFrames::STAND_LEFT);

    _x_collision_timer = 0;
}

MainDudeBaseState *MainDudeStandingState::update(MainDude& main_dude, World* world, uint32_t delta_time_ms)
{
    // Update components:

    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();
    auto* animation = main_dude.get_animation_component();

    assert(physics);
    assert(quad);
    assert(animation);

    physics->update(world, delta_time_ms);
    quad->update(physics->get_x_position(), physics->get_y_position(), !main_dude._other.facing_left);

    // Other:

    if (physics->get_x_velocity() != 0.0f)
    {
        return &main_dude._states.running;
    }

    if (physics->get_y_velocity() > 0.0f)
    {
        return &main_dude._states.falling;
    }
    else if (physics->get_y_velocity() < 0.0f)
    {
        return &main_dude._states.jumping;
    }

    if (physics->is_left_collision() || physics->is_right_collision())
    {
        _x_collision_timer += delta_time_ms;
        if (_x_collision_timer > 400)
        {
            _x_collision_timer = 0;
            return &main_dude._states.pushing;
        }
    }
    else
    {
        _x_collision_timer = 0;
    }

    return this;
}

MainDudeBaseState *MainDudeStandingState::handle_input(MainDude& main_dude, World* world, const Input &input)
{
    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(quad);

    if (input.left().value())
    {
        physics->add_velocity(-MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    if (input.right().value())
    {
        physics->add_velocity(MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    if (input.jumping().changed() && input.jumping().value())
    {
        physics->add_velocity(0.0f, -MainDude::JUMP_SPEED);
        return &main_dude._states.jumping;
    }
    if (input.ducking().value())
    {
        return &main_dude._states.ducking;
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

        return &main_dude._states.looking_up;
    }

    return this;
}
