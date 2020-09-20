#include "main-dude/states/MainDudeDeadState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"
#include "other/Inventory.hpp"

void MainDudeDeadState::enter(MainDude& main_dude)
{
    main_dude.notify(MainDudeEvent::DIED);
    main_dude._other.dead = true;

    if (main_dude._physics.get_y_velocity() == 0.0f)
    {
        // Give him a slight jump, just for entertainment:
        main_dude._physics.add_velocity(0.0f, -0.07f);
    }

    main_dude._physics.set_friction(PhysicsComponent::get_default_friction() * 1.8f);
    main_dude._physics.set_bounciness(0.5f);
    main_dude._physics.set_dimensions(main_dude._physics.get_width(), main_dude._physics.get_height());
    main_dude._animation.stop();
    set_current_frame(main_dude);

    // TODO: Change physical size of the main dude (about half of the height when dead).
    //       This can be done only after convention for storing position is changed - right now physics component's
    //       XY describes center of the body. Should be changed to describe upper-left corner.
}

MainDudeBaseState* MainDudeDeadState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(delta_time_ms);
    main_dude._quad.update(main_dude.get_x_pos_center(), main_dude.get_y_pos_center(), !main_dude._other.facing_left);
    set_current_frame(main_dude);

    return this;
}

MainDudeBaseState *MainDudeDeadState::handle_input(MainDude& main_dude, const Input &input)
{
    return this;
}

void MainDudeDeadState::set_current_frame(MainDude& main_dude)
{
    if (main_dude._physics.get_y_velocity() > 0.0f)
    {
        if (main_dude._quad.get_current_frame<MainDudeSpritesheetFrames>() != MainDudeSpritesheetFrames::DEAD_FALLING)
        {
            main_dude._quad.frame_changed(MainDudeSpritesheetFrames::DEAD_FALLING);
        }
    }
    else if (main_dude._physics.get_y_velocity() < 0.0f)
    {
        if (main_dude._quad.get_current_frame<MainDudeSpritesheetFrames>() != MainDudeSpritesheetFrames::DEAD_BOUNCE)
        {
            main_dude._quad.frame_changed(MainDudeSpritesheetFrames::DEAD_BOUNCE);
        }
    }
    else
    {
        if (main_dude._quad.get_current_frame<MainDudeSpritesheetFrames>() != MainDudeSpritesheetFrames::DEAD)
        {
            main_dude._quad.frame_changed(MainDudeSpritesheetFrames::DEAD);
        }
    }
}

void MainDudeDeadState::exit(MainDude& main_dude)
{
    main_dude._physics.set_friction(PhysicsComponent::get_default_friction());
    main_dude._physics.set_bounciness(0.0f);
    main_dude._other.dead = false;
    Inventory::instance().set_starting_inventory();
}
