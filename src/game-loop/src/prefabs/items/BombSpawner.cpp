#include "prefabs/items/BombSpawner.hpp"
#include "prefabs/items/Bomb.hpp"

#include "components/generic/ActivableComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"

#include "EntityRegistry.hpp"

entt::entity prefabs::BombSpawner::create(int initial_bomb_count)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();
    
    ActivableComponent activable;
    activable.activate_combination = { InputEvent::OUT_BOMB_PRESSED };

    registry.emplace<ItemComponent>(entity, ItemType::BOMB_SPAWNER, ItemApplication::ACTIVABLE, ItemSlot::OTHER);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<BombSpawnerScript>(initial_bomb_count));
    registry.emplace<ActivableComponent>(entity, activable);

    return entity;
}

void prefabs::BombSpawner::BombSpawnerScript::update(entt::entity owner, uint32_t delta_time_ms)
{
    auto &registry = EntityRegistry::instance().get_registry();
    auto &activable = registry.get<ActivableComponent>(owner);

    if (activable.activated)
    {
        auto &item = registry.get<ItemComponent>(owner);
        auto &item_carrier = item.get_item_carrier();

        if (_bomb_counter == 0)
        {
            return;
        }

        if (!item_carrier.has_active_item())
        {
            auto bomb = prefabs::Bomb::create();

            // References may become invalid after creating new components (pool resize) - refreshing:
            item = registry.get<ItemComponent>(owner);
            item_carrier = item.get_item_carrier();

            item_carrier.pick_up_item(bomb, item.get_item_carrier_entity());

            remove_bombs(1);
        }
    }
}
