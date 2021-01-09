#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudePushingState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "Input.hpp"

void MainDudePushingState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::PUSH_LEFT_0_FIRST),
                    static_cast<std::size_t>(MainDudeSpritesheetFrames::PUSH_LEFT_6_LAST),
                    75, true);
}

MainDudeBaseState* MainDudePushingState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    const auto& input = Input::instance();

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& position = registry.get<PositionComponent>(owner);
    auto& orientation = registry.get<HorizontalOrientationComponent>(owner);

    if (input.left().value())
    {
        physics.set_x_velocity(physics.get_x_velocity() - MainDudeComponent::DEFAULT_DELTA_X);
    }
    else if (orientation.orientation == HorizontalOrientation::LEFT)
    {
        return &dude._states.running;
    }

    if (input.right().value())
    {
        physics.set_x_velocity(physics.get_x_velocity() + MainDudeComponent::DEFAULT_DELTA_X);
    }
    else if (orientation.orientation == HorizontalOrientation::RIGHT)
    {
        return &dude._states.running;
    }

    if (input.jumping().changed() && input.jumping().value())
    {
        physics.set_y_velocity(physics.get_y_velocity() - MainDudeComponent::JUMP_SPEED);
        return &dude._states.jumping;
    }

    if (input.ducking().value())
    {
        return &dude._states.ducking;
    }

    if (!physics.is_left_collision() && !physics.is_right_collision())
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

    return this;
}
