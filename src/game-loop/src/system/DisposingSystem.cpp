#include "system/DisposingSystem.hpp"
#include "components/generic/TimeLimitComponent.hpp"
#include "EntityRegistry.hpp"

void DisposingSystem::update(std::uint32_t delta_time_ms)
{
    auto &registry = EntityRegistry::instance().get_registry();
    registry.view<TimeLimitComponent>().each([&delta_time_ms, &registry](entt::entity e, TimeLimitComponent& c){
        c.time_to_dispose_ms -= delta_time_ms;
        if (c.time_to_dispose_ms < 0)
        {
            registry.destroy(e);
        }
    });
}
