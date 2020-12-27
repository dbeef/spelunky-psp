#include "main-dude/states/MainDudeStunnedState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"
#include "audio/Audio.hpp"

namespace
{
    constexpr float STUNNED_TIME_MS = 3500;
}

void MainDudeStunnedState::enter(MainDude &main_dude)
{
    Audio::instance().play(SFXType::MAIN_DUDE_HURT);

    auto* physics = main_dude.get_physics_component();
    auto* animation = main_dude.get_animation_component();

    assert(physics);
    assert(animation);

    physics->set_friction(PhysicsComponent::get_default_friction() * 1.8f);
    animation->start(static_cast<std::size_t>(MainDudeSpritesheetFrames::STUNNED_0_FIRST),
                     static_cast<std::size_t>(MainDudeSpritesheetFrames::STUNNED_4_LAST),
                     75, true);

    _stunned_timer_ms = 0;
}

MainDudeBaseState *MainDudeStunnedState::update(MainDude& main_dude, World* world, uint32_t delta_time_ms)
{
    auto* physics = main_dude.get_physics_component();
    auto* animation = main_dude.get_animation_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(animation);
    assert(quad);

    physics->update(world, delta_time_ms);
    quad->update(physics->get_x_position(), physics->get_y_position(), !main_dude._other.facing_left);
    animation->update(*quad, delta_time_ms);

    _stunned_timer_ms += delta_time_ms;

    if (_stunned_timer_ms > STUNNED_TIME_MS)
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

MainDudeBaseState *MainDudeStunnedState::handle_input(MainDude& main_dude, World* world, const Input &input)
{
    return this;
}

void MainDudeStunnedState::exit(MainDude& main_dude)
{
    auto* physics = main_dude.get_physics_component();
    assert(physics);

    physics->set_friction(PhysicsComponent::get_default_friction());
}
