#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudeThrowingState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "Input.hpp"
#include "audio/Audio.hpp"

void MainDudeThrowingState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::THROWING_LEFT_0_FIRST),
                    static_cast<std::size_t>(MainDudeSpritesheetFrames::THROWING_LEFT_8_LAST),
                    50, false);

    Audio::instance().play(SFXType::MAIN_DUDE_WHIP);
}

MainDudeBaseState *MainDudeThrowingState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    const auto& input = Input::instance();

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& position = registry.get<PositionComponent>(owner);

    if (animation.is_finished())
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

    if (input.left().value())
    {
        physics.set_x_velocity(physics.get_x_velocity() - MainDudeComponent::DEFAULT_DELTA_X);
    }

    if (input.right().value())
    {
        physics.set_x_velocity(physics.get_x_velocity() + MainDudeComponent::DEFAULT_DELTA_X);
    }

    if (input.jumping().changed() && input.jumping().value() && physics.is_bottom_collision())
    {
        physics.set_y_velocity(-MainDudeComponent::JUMP_SPEED);
    }

    if (input.running_fast().value())
    {
        physics.set_max_x_velocity(MainDudeComponent::MAX_RUNNING_VELOCITY_X);
    }
    else
    {
        physics.set_max_x_velocity(MainDudeComponent::DEFAULT_MAX_X_VELOCITY);
    }

    return this;
}
