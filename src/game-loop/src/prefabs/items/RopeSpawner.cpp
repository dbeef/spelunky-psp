#include "prefabs/items/RopeSpawner.hpp"
#include "prefabs/items/Rope.hpp"

#include "components/generic/ActivableComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"

#include "EntityRegistry.hpp"

namespace
{
    class RopeSpawnerScript : public ScriptBase
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

                if (!item_carrier.has_active_item())
                {
                    auto rope = prefabs::Rope::create();

                    // References may become invalid after creating new components (pool resize) - refreshing:
                    item = registry.get<ItemComponent>(owner);
                    item_carrier = item.get_item_carrier();

                    item_carrier.pick_up_item(rope, item.get_item_carrier_entity());
                }
            }
        }
    };
}

entt::entity prefabs::RopeSpawner::create()
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    ActivableComponent activable;
    activable.activate_combination = { InputEvent::OUT_ROPE_PRESSED };

    registry.emplace<ItemComponent>(entity, ItemType::ACTIVABLE, ItemSlot::OTHER);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<RopeSpawnerScript>());
    registry.emplace<ActivableComponent>(entity, activable);

    return entity;
}
