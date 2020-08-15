#include "main-dude/states/MainDudeStunnedState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

namespace
{
    constexpr float STUNNED_TIME_MS = 3500;
}

void MainDudeStunnedState::enter(MainDude &main_dude)
{
    main_dude.decrease_hearts(1);
    main_dude._physics.set_friction(PhysicsComponent::get_default_friction() * 1.8f);
    main_dude._animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::STUNNED_0_FIRST),
                               static_cast<std::size_t>(MainDudeSpritesheetFrames::STUNNED_4_LAST),
                               75, true);
    _stunned_timer_ms = 0;
}

MainDudeBaseState *MainDudeStunnedState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude.get_x_pos_center(), main_dude.get_y_pos_center(), !main_dude._other.facing_left);
    main_dude._animation.update(main_dude, delta_time_ms);

    // Other:

    _stunned_timer_ms += delta_time_ms;

    if (_stunned_timer_ms > STUNNED_TIME_MS)
    {
        if (main_dude._physics.is_bottom_collision())
        {
            if (main_dude._physics.get_x_velocity() == 0.0f)
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
            if (main_dude._physics.get_y_velocity() > 0.0f)
            {
                return &main_dude._states.falling;
            }
            else if (main_dude._physics.get_y_velocity() < 0.0f)
            {
                return &main_dude._states.jumping;
            }
        }
    }

    return this;
}

MainDudeBaseState *MainDudeStunnedState::handle_input(MainDude& main_dude, const Input &input)
{
    return this;
}

void MainDudeStunnedState::exit(MainDude& main_dude)
{
    main_dude._physics.set_friction(PhysicsComponent::get_default_friction());
}
