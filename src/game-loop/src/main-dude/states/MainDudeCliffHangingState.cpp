#include "Level.hpp"
#include "main-dude/MainDude.hpp"
#include "main-dude/states/MainDudeCliffHangingState.hpp"
#include "Input.hpp"

void MainDudeCliffHangingState::enter(MainDude& main_dude)
{
    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(quad);

    physics->set_velocity(0.0f, 0.0f);
    physics->disable_gravity();
    quad->frame_changed(MainDudeSpritesheetFrames::HANGING_LEFT);
}

MainDudeBaseState* MainDudeCliffHangingState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(quad);

    physics->update(delta_time_ms);
    quad->update(physics->get_x_position(), physics->get_y_position(), !main_dude._other.facing_left);

    return this;
}

MainDudeBaseState *MainDudeCliffHangingState::handle_input(MainDude& main_dude, const Input &input)
{
    auto* physics = main_dude.get_physics_component();
    assert(physics);

    if (input.jumping().changed() && input.jumping().value())
    {
        // In the original game, when jumped from a cliff, main dude was moved one pixel opposite of the faced side.
        // I assume this is to not have a collision and enter cliff-hanging-state immediately after exiting it,
        // as it actually helps Spelunky-PSP's implementation this way.

        float offset = 1.0f / 16.0f;

        if (main_dude._other.facing_left)
        {
            physics->add_position(offset, 0.0f);
        } else
        {
            physics->add_position(-offset, 0.0f);
        }

        physics->add_velocity(0.0f, -MainDude::JUMP_SPEED);
        physics->enable_gravity();
        return &main_dude._states.jumping;
    }

    return this;
}

void MainDudeCliffHangingState::exit(MainDude& main_dude)
{
    auto* physics = main_dude.get_physics_component();
    assert(physics);

    physics->enable_gravity();
}
