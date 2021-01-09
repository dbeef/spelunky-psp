#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudeLookingUpState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include <components/generic/InputComponent.hpp>
#include "Input.hpp"

void MainDudeLookingUpState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& input = registry.get<InputComponent>(owner);

    input.allowed_events = {
            InputEvent::JUMPING,
            InputEvent::JUMPING_PRESSED,
            InputEvent::LEFT,
            InputEvent::LEFT_PRESSED,
            InputEvent::RIGHT,
            InputEvent::RIGHT_PRESSED,
            InputEvent::RUNNING_FAST,
            InputEvent::RUNNING_FAST_PRESSED,
            InputEvent::THROWING,
            InputEvent::THROWING_PRESSED,
            InputEvent::OUT_BOMB,
            InputEvent::OUT_BOMB_PRESSED,
            InputEvent::OUT_ROPE,
            InputEvent::OUT_ROPE_PRESSED,
            InputEvent::UP,
            InputEvent::UP_PRESSED,
    };

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

    if (input.jumping().changed() && input.jumping().value())
    {
        return &dude._states.jumping;
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
