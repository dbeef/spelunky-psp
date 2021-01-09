#include "EntityRegistry.hpp"
#include "main-dude/states/MainDudeLevelSummaryState.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include <components/generic/InputComponent.hpp>
#include "Input.hpp"

void MainDudeLevelSummaryState::enter(MainDudeComponent& dude)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& input = registry.get<InputComponent>(owner);

    input.allowed_events = {};

    physics.set_max_x_velocity(MainDudeComponent::DEFAULT_MAX_X_VELOCITY);
    animation.start(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST),
                    static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_5_LAST),
                    75, true);
}

MainDudeBaseState* MainDudeLevelSummaryState::update(MainDudeComponent& dude, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();
    const auto& owner = dude._owner;

    auto& animation = registry.get<AnimationComponent>(owner);
    auto& physics = registry.get<PhysicsComponent>(owner);
    auto& quad = registry.get<QuadComponent>(owner);
    auto& position = registry.get<PositionComponent>(owner);

    physics.set_x_velocity(0.095f);
    physics.set_y_velocity(0);

    const auto* exit_tile = dude.is_overlaping_tile(MapTileType::EXIT, physics, position);
    if (exit_tile)
    {
        position.set_position_on_tile(exit_tile);
        return &dude._states.exiting;
    }

    return this;
}
