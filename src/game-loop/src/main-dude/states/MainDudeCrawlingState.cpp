#include "main-dude/states/MainDudeCrawlingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeCrawlingState::enter(MainDude& main_dude)
{
    auto* physics = main_dude.get_physics_component();
    auto* animation = main_dude.get_animation_component();

    assert(physics);
    assert(animation);

    physics->set_max_x_velocity(MainDude::MAX_CRAWLING_VELOCITY_X);
    animation->start(static_cast<std::size_t>(MainDudeSpritesheetFrames::CRAWLING_LEFT_0_FIRST),
                     static_cast<std::size_t>(MainDudeSpritesheetFrames::CRAWLING_LEFT_9_LAST),
                     75, true);
}

MainDudeBaseState* MainDudeCrawlingState::update(MainDude& main_dude, uint32_t delta_time_ms)
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
        return &main_dude._states.ducking;
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

MainDudeBaseState *MainDudeCrawlingState::handle_input(MainDude& main_dude, const Input &input)
{
    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(quad);

    if (input.left().value())
    {
        physics->add_velocity(-MainDude::CRAWLING_DELTA_X, 0.0f);
    }
    if (input.right().value())
    {
        physics->add_velocity(MainDude::CRAWLING_DELTA_X, 0.0f);
    }
    if (input.jumping().changed() && input.jumping().value())
    {
        physics->add_velocity(0.0f, -MainDude::JUMP_SPEED);
        return &main_dude._states.jumping;
    }
    if (!input.ducking().value())
    {
        return &main_dude._states.running;
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
            physics->set_position(
                    exit_tile->x + quad->get_quad_width() / 2,
                    exit_tile->y + quad->get_quad_height() / 2);

            return &main_dude._states.exiting;
        }
    }

    return this;
}
