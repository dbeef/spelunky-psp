#include "main-dude/states/MainDudeThrowingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"
#include "audio/Audio.hpp"

void MainDudeThrowingState::enter(MainDude &main_dude)
{
    Audio::instance().play(SFXType::MAIN_DUDE_WHIP);

    auto* animation = main_dude.get_animation_component();
    assert(animation);

    animation->start(static_cast<std::size_t>(MainDudeSpritesheetFrames::THROWING_LEFT_0_FIRST),
                     static_cast<std::size_t>(MainDudeSpritesheetFrames::THROWING_LEFT_8_LAST),
                     50, false);
}

MainDudeBaseState *MainDudeThrowingState::update(MainDude& main_dude, World* world, uint32_t delta_time_ms)
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

    if (animation->is_finished())
    {
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
        else
        {
            if (physics->get_y_velocity() > 0.0f)
            {
                return &main_dude._states.falling;
            }
            else if (physics->get_y_velocity() < 0.0f)
            {
                return &main_dude._states.jumping;
            }
        }
    }

    return this;
}

MainDudeBaseState *MainDudeThrowingState::handle_input(MainDude& main_dude, World* world, const Input &input)
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
    if (input.jumping().changed() && input.jumping().value() && physics->is_bottom_collision())
    {
        physics->add_velocity(0.0f, -MainDude::JUMP_SPEED);
    }

    if (input.running_fast().value())
    {
        physics->set_max_x_velocity(MainDude::MAX_RUNNING_VELOCITY_X);
    }
    else
    {
        physics->set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);
    }

    return this;
}
