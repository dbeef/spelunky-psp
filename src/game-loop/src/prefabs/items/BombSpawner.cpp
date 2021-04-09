#include "prefabs/items/BombSpawner.hpp"
#include "prefabs/items/Bomb.hpp"

#include "components/generic/ActivableComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"

#include "EntityRegistry.hpp"
#include "other/Inventory.hpp"

namespace
{
    class BombSpawnerScript final : public ScriptBase
    {
    public:

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& activable = registry.get<ActivableComponent>(owner);

            if (activable.activated)
            {
                auto& item = registry.get<ItemComponent>(owner);
                auto& item_carrier = item.get_item_carrier();

                // TODO: Spawning bombs should be limited to the amount in inventory (same for ropes)
                //       Problem - how to do it in generic way, i.e to not hard-wire it to main dude inventory?
                //
                // Main dude could subscribe on inventory event to know when amount of bombs changed,
                // then it could check if the item he has is a bomb spawner and update its own counter?
                //
                // OR
                //
                // BombSpawner should be removed/added to main dude inventory dynamically (on inventory event),
                // instead of once forever until main dude disposed - but then removing bomb should also notify inventory
                //
                //
                // So far Inventory is a singleton (to save state between the levels)
                // How about Inventory singleton + InventoryComponent that would mirror it plus enable easy access
                // for Bomb/Rope spawner through subject/observer?
                //
                // OR
                //
                // Just don't over-engineer and hard-wire it as technically main dude is the only thing in the
                // game that can spawn bombs / ropes:
                if (Inventory::instance().get_bombs() == 0)
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

                    Inventory::instance().remove_bombs(1);
                }
            }
        }
    };
}

entt::entity prefabs::BombSpawner::create()
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();
    
    ActivableComponent activable;
    activable.activate_combination = { InputEvent::OUT_BOMB_PRESSED };

    registry.emplace<ItemComponent>(entity, ItemType::BOMB_SPAWNER, ItemApplication::ACTIVABLE, ItemSlot::OTHER);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<BombSpawnerScript>());
    registry.emplace<ActivableComponent>(entity, activable);

    return entity;
}
