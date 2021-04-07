#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudeRunningLookingUpState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include <components/generic/InputComponent.hpp>
#include "Input.hpp"

void MainDudeRunningLookingUpState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);
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

    if (dude._states.current == &dude._states.running)
    {
        animation.resume(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUNNING_LOOKING_UP_0_FIRST),
                         static_cast<std::size_t>(MainDudeSpritesheetFrames::RUNNING_LOOKING_UP_5_LAST));
    }
    else
    {
        animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUNNING_LOOKING_UP_0_FIRST),
                        static_cast<std::size_t>(MainDudeSpritesheetFrames::RUNNING_LOOKING_UP_5_LAST),
                        75, true);
    }
}

MainDudeBaseState *MainDudeRunningLookingUpState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
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

    if (input.ducking().value())
    {
        return &dude._states.crawling;
    }

    if (input.running_fast().value())
    {
        animation.set_time_per_frame_ms(50);
    }
    else
    {
        animation.set_time_per_frame_ms(75);
    }

    if (input.up().value())
    {
        if (physics.get_x_velocity() == 0.0f)
        {
            return &dude._states.looking_up;
        }

        const auto* exit_tile = dude.is_overlaping_tile(MapTileType::EXIT, physics, position);

        if (exit_tile)
        {
            position.set_position_on_tile(exit_tile);
            return &dude._states.exiting;
        }

        return this;
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

    return &dude._states.running;
}
