#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudeStandingState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include <components/generic/InputComponent.hpp>
#include "Input.hpp"
#include "logger/log.h"

void MainDudeStandingState::enter(MainDudeComponent& dude)
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
            InputEvent::ACCEPT_TRANSACTION,
            InputEvent::ACCEPT_TRANSACTION_PRESSED,
    };

    animation.stop();
    quad.frame_changed(MainDudeSpritesheetFrames::STAND_LEFT);

    _x_collision_timer = 0;
}

MainDudeBaseState *MainDudeStandingState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
{
    const auto& input = Input::instance();
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& position = registry.get<PositionComponent>(owner);

    if (physics.get_y_velocity() < 0.0f)
    {
        return &dude._states.jumping;
    }

    if (input.ducking().value())
    {
        return &dude._states.ducking;
    }

    if (input.up().value())
    {
        const auto* exit_tile = dude.is_overlaping_tile(MapTileType::EXIT, physics, position);

        if (exit_tile)
        {
            position.set_position_on_tile(exit_tile);
            return &dude._states.exiting;
        }

        return &dude._states.looking_up;
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

    if (physics.is_left_collision() || physics.is_right_collision())
    {
        _x_collision_timer += delta_time_ms;
        if (_x_collision_timer > 400)
        {
            _x_collision_timer = 0;
            return &dude._states.pushing;
        }
    }
    else
    {
        _x_collision_timer = 0;
    }

    return this;
}
