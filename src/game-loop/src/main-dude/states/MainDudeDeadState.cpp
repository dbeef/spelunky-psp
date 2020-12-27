#include "main-dude/states/MainDudeDeadState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"
#include "other/Inventory.hpp"
#include "audio/Audio.hpp"

void MainDudeDeadState::enter(MainDude& main_dude)
{
    Audio::instance().stop();
    Audio::instance().play(SFXType::MAIN_DUDE_DIE);

    main_dude.notify(MainDudeEvent::DIED);
    main_dude._other.dead = true;

    auto* physics = main_dude.get_physics_component();
    auto* animation = main_dude.get_animation_component();

    assert(physics);
    assert(animation);

    if (physics->get_y_velocity() == 0.0f)
    {
        // Give him a slight jump, just for entertainment:
        physics->add_velocity(0.0f, -0.07f);
    }

    physics->set_friction(PhysicsComponent::get_default_friction() * 1.8f);
    physics->set_bounciness(0.5f);
    physics->set_dimensions(physics->get_width(), physics->get_height());
    animation->stop();
    set_current_frame(main_dude);

    // TODO: Change physical size of the main dude (about half of the height when dead).
    //       This can be done only after convention for storing position is changed - right now physics component's
    //       XY describes center of the body. Should be changed to describe upper-left corner.
}

MainDudeBaseState* MainDudeDeadState::update(MainDude& main_dude, World* world, uint32_t delta_time_ms)
{
    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(quad);

    physics->update(world, delta_time_ms);
    quad->update(physics->get_x_position(), physics->get_y_position(), !main_dude._other.facing_left);
    set_current_frame(main_dude);

    return this;
}

MainDudeBaseState *MainDudeDeadState::handle_input(MainDude& main_dude, World* world, const Input &input)
{
    return this;
}

void MainDudeDeadState::set_current_frame(MainDude& main_dude)
{
    auto* physics = main_dude.get_physics_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(quad);

    if (physics->get_y_velocity() > 0.0f)
    {
        if (quad->get_current_frame<MainDudeSpritesheetFrames>() != MainDudeSpritesheetFrames::DEAD_FALLING)
        {
            quad->frame_changed(MainDudeSpritesheetFrames::DEAD_FALLING);
        }
    }
    else if (physics->get_y_velocity() < 0.0f)
    {
        if (quad->get_current_frame<MainDudeSpritesheetFrames>() != MainDudeSpritesheetFrames::DEAD_BOUNCE)
        {
            quad->frame_changed(MainDudeSpritesheetFrames::DEAD_BOUNCE);
        }
    }
    else
    {
        if (quad->get_current_frame<MainDudeSpritesheetFrames>() != MainDudeSpritesheetFrames::DEAD)
        {
            quad->frame_changed(MainDudeSpritesheetFrames::DEAD);
        }
    }
}

void MainDudeDeadState::exit(MainDude& main_dude)
{
    auto* physics = main_dude.get_physics_component();
    assert(physics);

    physics->set_friction(PhysicsComponent::get_default_friction());
    physics->set_bounciness(0.0f);
    main_dude._other.dead = false;
    Inventory::instance().set_starting_inventory();
}
