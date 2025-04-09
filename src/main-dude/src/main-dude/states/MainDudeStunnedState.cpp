#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudeStunnedState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include <components/generic/InputComponent.hpp>
#include "Input.hpp"
#include "audio/Audio.hpp"

namespace
{
    constexpr float STUNNED_TIME_MS = 3500;
}

void MainDudeStunnedState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& input = registry.get<InputComponent>(owner);

    input.allowed_events = {};

    physics.set_friction(PhysicsComponent::get_default_friction() * 1.8f);
    animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::STUNNED_0_FIRST),
                    static_cast<std::size_t>(MainDudeSpritesheetFrames::STUNNED_4_LAST),
                    75, true);

    Audio::instance().play(SFXType::MAIN_DUDE_HURT);
    _stunned_timer_ms = 0;
    if (physics.get_y_velocity() == 0.0f)
    {
        // Give him a slight jump, just for entertainment:
        physics.set_y_velocity(physics.get_y_velocity() - 0.07f);
    }
}

MainDudeBaseState *MainDudeStunnedState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
{
    // TODO: Depending on bottom collision, stunned state should change frame (see MainDudeDeadState).

    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& position = registry.get<PositionComponent>(owner);

    _stunned_timer_ms += delta_time_ms;

    if (_stunned_timer_ms > STUNNED_TIME_MS)
    {
        if (physics.is_bottom_collision())
        {
            if (physics.get_x_velocity() == 0.0f)
            {
                return &dude._states.standing;
            }
            else
            {
                return &dude._states.running;
            }
        }
        else
        {
            if (physics.get_y_velocity() > 0.0f)
            {
                return &dude._states.falling;
            }
            else if (physics.get_y_velocity() < 0.0f)
            {
                return &dude._states.jumping;
            }
        }
    }

    return this;
}

void MainDudeStunnedState::exit(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& position = registry.get<PositionComponent>(owner);

    physics.set_friction(PhysicsComponent::get_default_friction());
}
