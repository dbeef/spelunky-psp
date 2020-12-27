#include "main-dude/states/MainDudeDuckingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeDuckingState::enter(MainDude& main_dude)
{
    auto* animation = main_dude.get_animation_component();
    auto* quad = main_dude.get_quad_component();

    assert(animation);
    assert(quad);

    animation->stop();
    quad->frame_changed(MainDudeSpritesheetFrames::CRAWLING_LEFT_0_FIRST);
}

MainDudeBaseState* MainDudeDuckingState::update(MainDude& main_dude, World* world, uint32_t delta_time_ms)
{
    auto* animation = main_dude.get_animation_component();
    auto* quad = main_dude.get_quad_component();
    auto* physics = main_dude.get_physics_component();

    assert(animation);
    assert(quad);
    assert(physics);

    physics->update(world, delta_time_ms);
    quad->update(physics->get_x_position(), physics->get_y_position(), !main_dude._other.facing_left);

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

    return this;
}

MainDudeBaseState *MainDudeDuckingState::handle_input(MainDude& main_dude, World* world, const Input &input)
{
    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(quad);

    if (input.left().value())
    {
        physics->add_velocity(-MainDude::DEFAULT_DELTA_X, 0.0f);
        return &main_dude._states.crawling;
    }
    if (input.right().value())
    {
        physics->add_velocity(MainDude::DEFAULT_DELTA_X, 0.0f);
        return &main_dude._states.crawling;
    }
    if (input.jumping().changed() && input.jumping().value())
    {
        physics->add_velocity(0.0f, -MainDude::JUMP_SPEED);
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
        const auto* exit_tile = main_dude.is_overlaping_tile(world, MapTileType::EXIT);
        if (exit_tile)
        {
            physics->set_position(
                    exit_tile->x + quad->get_quad_width() / 2,
                    exit_tile->y + quad->get_quad_height() / 2);

            return &main_dude._states.exiting;
        }
    }

    return this;
}
