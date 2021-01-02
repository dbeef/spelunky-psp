#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudeLookingUpState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "Input.hpp"

void MainDudeLookingUpState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);

    animation.stop();
    quad.frame_changed(MainDudeSpritesheetFrames::STANDING_LOOKING_UP);
}

MainDudeBaseState* MainDudeLookingUpState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    const auto& input = Input::instance();

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& position = registry.get<PositionComponent>(owner);

    if (input.left().value())
    {
        physics.set_x_velocity(physics.get_x_velocity() - MainDudeComponent::DEFAULT_DELTA_X);
    }

    if (input.right().value())
    {
        physics.set_x_velocity(physics.get_x_velocity() + MainDudeComponent::DEFAULT_DELTA_X);
    }

    if (input.jumping().changed() && input.jumping().value())
    {
        physics.set_y_velocity(physics.get_y_velocity() - MainDudeComponent::JUMP_SPEED);
        return &dude._states.jumping;
    }

    if (input.throwing().changed() && input.throwing().value())
    {
        return &dude._states.throwing;
    }

    if (!input.up().value())
    {
        return &dude._states.standing;
    }

    if (physics.get_x_velocity() != 0.0f)
    {
        return &dude._states.running_looking_up;
    }

    if (physics.get_y_velocity() > 0.0f)
    {
        return &dude._states.falling;
    }
    else if (physics.get_y_velocity() < 0.0f)
    {
        return &dude._states.jumping;
    }

    return this;
}
