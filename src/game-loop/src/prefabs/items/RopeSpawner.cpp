#include "prefabs/items/RopeSpawner.hpp"
#include "prefabs/items/Rope.hpp"

#include "components/generic/ActivableComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"

#include "EntityRegistry.hpp"
#include "other/Inventory.hpp"

entt::entity prefabs::RopeSpawner::create(int initial_rope_count)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    ActivableComponent activable;
    activable.activate_combination = { InputEvent::OUT_ROPE_PRESSED };

    registry.emplace<ItemComponent>(entity, ItemType::ROPE_SPAWNER, ItemApplication::ACTIVABLE, ItemSlot::OTHER);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<RopeSpawnerScript>(initial_rope_count));
    registry.emplace<ActivableComponent>(entity, activable);

    return entity;
}

void prefabs::RopeSpawner::RopeSpawnerScript::update(entt::entity owner, uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();
    auto& activable = registry.get<ActivableComponent>(owner);

    if (activable.activated)
    {
        auto& item = registry.get<ItemComponent>(owner);
        auto& item_carrier = item.get_item_carrier();

        if (_rope_counter == 0)
        {
            return;
        }

        if (!item_carrier.has_active_item())
        {
            auto item_carrier_entity = item.get_item_carrier_entity();
            auto item_carrier_position = registry.get<PositionComponent>(item_carrier_entity);

            auto rope = prefabs::Rope::create(item_carrier_position.x_center, item_carrier_position.y_center);

            // References may become invalid after creating new components (pool resize) - refreshing:
            item = registry.get<ItemComponent>(owner);
            item_carrier = item.get_item_carrier();

            item_carrier.pick_up_item(rope, item.get_item_carrier_entity());

            remove_ropes(1);
        }
    }
}
