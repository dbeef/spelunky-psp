#include "system/ItemSystem.hpp"
#include "EntityRegistry.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"

void ItemSystem::update(std::uint32_t delta_time_ms)
{
    auto &registry = EntityRegistry::instance().get_registry();
    const auto item_carriers = registry.view<ItemCarrierComponent, PositionComponent, HorizontalOrientationComponent>();

    item_carriers.each([](ItemCarrierComponent& carrier, PositionComponent& position, HorizontalOrientationComponent& orientation)
    {
        carrier.update_carried_items_positions(position);
        carrier.update_carried_items_orientation(orientation);
    });
}
