#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudeDeadState.hpp"
#include <components/generic/InputComponent.hpp>
#include "components/specialized/MainDudeComponent.hpp"
#include "Input.hpp"
#include "other/Inventory.hpp"
#include "audio/Audio.hpp"

void MainDudeDeadState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& animation = registry.get<AnimationComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& input = registry.get<InputComponent>(owner);

    input.allowed_events = {};

    if (physics.get_y_velocity() == 0.0f)
    {
        // Give him a slight jump, just for entertainment:
        physics.set_y_velocity(physics.get_y_velocity() - 0.07f);
    }

    physics.set_friction(PhysicsComponent::get_default_friction() * 1.8f);
    physics.set_bounciness(0.5f);
    animation.stop();
    set_current_frame(quad, physics);

    Audio::instance().stop();
    Audio::instance().play(SFXType::MAIN_DUDE_DIE);

    dude.notify(MainDudeEvent::DIED);
    dude._other.dead = true;

    // TODO: Change physical size of the main dude (about half of the height when dead).
    //       This can be done only after convention for storing position is changed - right now physics component's
    //       XY describes center of the body. Should be changed to describe upper-left corner.
}

MainDudeBaseState* MainDudeDeadState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);

    set_current_frame(quad, physics);

    return this;
}

void MainDudeDeadState::set_current_frame(QuadComponent& quad, PhysicsComponent& physics)
{
    if (physics.get_y_velocity() > 0.0f)
    {
        if (quad.get_current_frame<MainDudeSpritesheetFrames>() != MainDudeSpritesheetFrames::DEAD_FALLING)
        {
            quad.frame_changed(MainDudeSpritesheetFrames::DEAD_FALLING);
        }
    }
    else if (physics.get_y_velocity() < 0.0f)
    {
        if (quad.get_current_frame<MainDudeSpritesheetFrames>() != MainDudeSpritesheetFrames::DEAD_BOUNCE)
        {
            quad.frame_changed(MainDudeSpritesheetFrames::DEAD_BOUNCE);
        }
    }
    else
    {
        if (quad.get_current_frame<MainDudeSpritesheetFrames>() != MainDudeSpritesheetFrames::DEAD)
        {
            quad.frame_changed(MainDudeSpritesheetFrames::DEAD);
        }
    }
}

void MainDudeDeadState::exit(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& physics = registry.get<PhysicsComponent>(owner);

    physics.set_friction(PhysicsComponent::get_default_friction());
    physics.set_bounciness(0.0f);
    dude._other.dead = false;
    Inventory::instance().set_starting_inventory();
}
