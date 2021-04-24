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

                // FIXME: Wiring bomb counter directly with main-dude's inventory. Technically not wrong, as main-dude
                //        is the only thing spawning bombs in the game, but as existing codebase tries not to make
                //        assumptions on item users, this should be the case here too.
                //        Potential house-keeping task - find not-so-overcomplicated method of decoupling Inventory with
                //        BombSpawner. Same goes for the RopeSpawner, and possibly even Crate (adding ropes/bombs).
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
