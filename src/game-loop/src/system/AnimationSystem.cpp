#include "system/AnimationSystem.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "EntityRegistry.hpp"

void AnimationSystem::update(std::uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();

    auto view = registry.view<AnimationComponent, QuadComponent>();
    view.each([&delta_time_ms](entt::entity entity, AnimationComponent& animation, QuadComponent& quad)
    {
        animation.update(quad, delta_time_ms);
    });
}

