#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudeCrawlingState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "Input.hpp"

void MainDudeCrawlingState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& animation = registry.get<AnimationComponent>(owner);

    physics.set_max_x_velocity(MainDudeComponent::MAX_CRAWLING_VELOCITY_X);
    animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::CRAWLING_LEFT_0_FIRST),
                    static_cast<std::size_t>(MainDudeSpritesheetFrames::CRAWLING_LEFT_9_LAST),
    75, true);
}

MainDudeBaseState* MainDudeCrawlingState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    const auto& input = Input::instance();

    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& position = registry.get<PositionComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);

    if (input.left().value())
    {
        physics.set_x_velocity(physics.get_x_velocity() - MainDudeComponent::CRAWLING_DELTA_X);
    }

    if (input.right().value())
    {
        physics.set_x_velocity(physics.get_x_velocity() + MainDudeComponent::CRAWLING_DELTA_X);
    }

    if (input.jumping().changed() && input.jumping().value())
    {
        physics.set_y_velocity(physics.get_y_velocity() - MainDudeComponent::JUMP_SPEED);
        return &dude._states.jumping;
    }

    if (!input.ducking().value())
    {
        return &dude._states.running;
    }

    if (input.throwing().changed() && input.throwing().value())
    {
        return &dude._states.throwing;
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

    if (physics.get_x_velocity() == 0.0f)
    {
        return &dude._states.ducking;
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
