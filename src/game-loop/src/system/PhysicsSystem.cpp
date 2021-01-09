#include "system/PhysicsSystem.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "Level.hpp"
#include "MapTile.hpp"
#include "Collisions.hpp"
#include "EntityRegistry.hpp"

#include <cmath>

namespace
{
    constexpr float SMALLEST_POSITION_STEP = 1.0f / 64.0f; // TODO: Should be a function of velocity
    constexpr float BOUNCE_EPSILON = 0.05f;
    constexpr float GRAVITY = 0.011f;
    constexpr uint16_t POS_UPDATE_DELTA_MS = 15;
}

void PhysicsSystem::update(std::uint32_t delta_time_ms)
{
    auto &registry = EntityRegistry::instance().get_registry();

    // Physics calculations:
    {
        auto view = registry.view<PhysicsComponent, PositionComponent>();
        view.each([&delta_time_ms](PhysicsComponent &physics, PositionComponent &position)
        {
            physics.update(delta_time_ms, position);
        });
    }
    // Update orientation:
    {
        auto view = registry.view<PhysicsComponent, HorizontalOrientationComponent>();
        view.each([&delta_time_ms](PhysicsComponent &physics, HorizontalOrientationComponent& orientation)
        {
            if (physics.get_x_velocity() < 0.0f)
            {
                orientation.orientation = HorizontalOrientation::LEFT;
            }
            else if (physics.get_x_velocity() > 0.0f)
            {
                orientation.orientation = HorizontalOrientation::RIGHT;
            }
        });
    }
}
