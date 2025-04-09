#include "EntityRegistry.hpp"
#include <components/generic/InputComponent.hpp>
#include "main-dude/states/MainDudeDuckingState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "Input.hpp"

void MainDudeDuckingState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& input = registry.get<InputComponent>(owner);

    input.allowed_events = {
            InputEvent::LEFT,
            InputEvent::LEFT_PRESSED,
            InputEvent::RIGHT,
            InputEvent::RIGHT_PRESSED,
            InputEvent::JUMPING,
            InputEvent::JUMPING_PRESSED,
            InputEvent::THROWING,
            InputEvent::THROWING_PRESSED,
            InputEvent::OUT_BOMB,
            InputEvent::OUT_BOMB_PRESSED,
            InputEvent::OUT_ROPE,
            InputEvent::OUT_ROPE_PRESSED,
    };

    animation.stop();
    quad.frame_changed(MainDudeSpritesheetFrames::CRAWLING_LEFT_0_FIRST);
}

MainDudeBaseState* MainDudeDuckingState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    const auto& input = Input::instance();

    auto& quad = registry.get<QuadComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& position = registry.get<PositionComponent>(owner);

    if (input.left().value())
    {
        return &dude._states.crawling;
    }

    if (input.right().value())
    {
        return &dude._states.crawling;
    }

    if (input.jumping().changed() && input.jumping().value())
    {
        return &dude._states.jumping;
    }

    if (!input.ducking().value())
    {
        return &dude._states.standing;
    }

    if (input.up().value())
    {
        const auto* exit_tile = dude.is_overlaping_tile(MapTileType::EXIT, physics, position);

        if (exit_tile)
        {
            position.set_position_on_tile(exit_tile);
            return &dude._states.exiting;
        }
    }

    if (physics.get_x_velocity() != 0.0f)
    {
        return &dude._states.running;
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
