#include "main-dude/states/MainDudeLookingUpState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeLookingUpState::enter(MainDude& main_dude)
{
    auto* animation = main_dude.get_animation_component();
    auto* quad = main_dude.get_quad_component();

    assert(animation);
    assert(quad);

    animation->stop();
    quad->frame_changed(MainDudeSpritesheetFrames::STANDING_LOOKING_UP);
}

MainDudeBaseState* MainDudeLookingUpState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(quad);

    physics->update(delta_time_ms);
    quad->update(physics->get_x_position(), physics->get_y_position(), !main_dude._other.facing_left);

    if (physics->get_x_velocity() != 0.0f)
    {
        return &main_dude._states.running_looking_up;
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

MainDudeBaseState *MainDudeLookingUpState::handle_input(MainDude& main_dude, const Input &input)
{
    auto* physics = main_dude.get_physics_component();
    assert(physics);

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
    if (input.throwing().changed() && input.throwing().value())
    {
        return &main_dude._states.throwing;
    }
    
    if (!input.up().value())
    {
        return &main_dude._states.standing;
    }

    return this;
}
