#include "main-dude/states/MainDudePushingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudePushingState::enter(MainDude& main_dude)
{
    auto* animation = main_dude.get_animation_component();
    assert(animation);

    animation->start(static_cast<std::size_t>(MainDudeSpritesheetFrames::PUSH_LEFT_0_FIRST),
                     static_cast<std::size_t>(MainDudeSpritesheetFrames::PUSH_LEFT_6_LAST),
                     75, true);
}

MainDudeBaseState* MainDudePushingState::update(MainDude& main_dude, World* world, uint32_t delta_time_ms)
{
    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();
    auto* animation = main_dude.get_animation_component();

    assert(physics);
    assert(quad);
    assert(animation);

    physics->update(world, delta_time_ms);
    quad->update(physics->get_x_position(), physics->get_y_position(), !main_dude._other.facing_left);
    animation->update(*quad, delta_time_ms);

    if (!physics->is_left_collision() && !physics->is_right_collision())
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

    return this;
}

MainDudeBaseState *MainDudePushingState::handle_input(MainDude& main_dude, World* world, const Input &input)
{
    auto* physics = main_dude.get_physics_component();
    assert(physics);

    if (input.left().value())
    {
        physics->add_velocity(-MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    else if (main_dude._other.facing_left)
    {
        return &main_dude._states.running;
    }

    if (input.right().value())
    {
        physics->add_velocity(MainDude::DEFAULT_DELTA_X, 0.0f);
    }
    else if (!main_dude._other.facing_left)
    {
        return &main_dude._states.running;
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

    return this;
}
