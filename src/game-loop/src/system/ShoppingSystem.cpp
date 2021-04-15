#include "system/ShoppingSystem.hpp"
#include "EntityRegistry.hpp"
#include "components/generic/SaleableComponent.hpp"
#include "components/generic/PositionComponent.hpp"

void ShoppingSystem::update(std::uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto saleable_items = registry.view<SaleableComponent, PositionComponent>();

    saleable_items.each([&registry](SaleableComponent& item_saleable, PositionComponent& item_position)
    {
        if (item_saleable.get_dollar_sign_animation() != entt::null)
        {
            item_saleable.update_position(item_position);
        }
    });
}
