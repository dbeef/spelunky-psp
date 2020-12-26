#include "main-dude/states/MainDudeRunningLookingUpState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeRunningLookingUpState::enter(MainDude &main_dude)
{
    auto* physics = main_dude.get_physics_component();
    auto* animation = main_dude.get_animation_component();

    assert(physics);
    assert(animation);

    physics->set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);

    if (main_dude._states.current == &main_dude._states.running)
    {
        animation->resume(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUNNING_LOOKING_UP_0_FIRST),
                          static_cast<std::size_t>(MainDudeSpritesheetFrames::RUNNING_LOOKING_UP_5_LAST));
    }
    else
    {
        animation->start(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUNNING_LOOKING_UP_0_FIRST),
                         static_cast<std::size_t>(MainDudeSpritesheetFrames::RUNNING_LOOKING_UP_5_LAST),
                         75, true);
    }
}

MainDudeBaseState *MainDudeRunningLookingUpState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    auto* physics = main_dude.get_physics_component();
    auto* animation = main_dude.get_animation_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(animation);
    assert(quad);

    physics->update(delta_time_ms);
    quad->update(physics->get_x_position(), physics->get_y_position(), !main_dude._other.facing_left);
    animation->update(*quad, delta_time_ms);

    if (physics->get_x_velocity() == 0.0f)
    {
        return &main_dude._states.looking_up;
    }

    if (physics->get_y_velocity() > 0.0f)
    {
        return &main_dude._states.falling;
    }
    else if (physics->get_y_velocity() < 0.0f)
    {
        return &main_dude._states.jumping;
    }

    return this;
}

MainDudeBaseState *MainDudeRunningLookingUpState::handle_input(MainDude& main_dude, const Input &input)
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
        return &main_dude._states.crawling;
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
        const auto* exit_tile = main_dude.is_overlaping_tile(MapTileType::EXIT);
        if (exit_tile)
        {
            physics->set_position(exit_tile->x + quad->get_quad_width() / 2, exit_tile->y + quad->get_quad_height() / 2);
            return &main_dude._states.exiting;
        }

        const auto* ladder_tile = main_dude.is_overlaping_tile(MapTileType::LADDER);
        const auto* ladder_deck_tile = main_dude.is_overlaping_tile(MapTileType::LADDER_DECK);

        if (ladder_tile || ladder_deck_tile)
        {
            const auto* tile = ladder_tile ? ladder_tile : ladder_deck_tile;
            physics->set_position(tile->x + quad->get_quad_width() / 2, physics->get_y_position());
            return &main_dude._states.climbing;
        }
        return this;
    }

    return &main_dude._states.running;
}
