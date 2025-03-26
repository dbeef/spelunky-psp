#include "prefabs/items/RopePile.hpp"
#include "prefabs/items/RopeSpawner.hpp"

#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/SaleableComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/OpenableComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/damage/GiveProjectileDamageComponent.hpp"

#include "audio/Audio.hpp"
#include "other/Inventory.hpp"
#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

namespace
{
    class RopePileScript final : public ScriptBase
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& item = registry.get<ItemComponent>(owner);

            if (item.is_carried() && !registry.has<SaleableComponent>(owner))
            {
                auto& item_carrier = item.get_item_carrier();
                item_carrier.put_down_active_item();

                const auto spawner = item_carrier.get_item(ItemType::ROPE_SPAWNER);

                if (spawner != entt::null)
                {
                    auto& rope_spawner_script_component = registry.get<ScriptingComponent>(spawner);
                    auto* rope_spawner_script = rope_spawner_script_component.get<prefabs::RopeSpawner::RopeSpawnerScript>();
                    rope_spawner_script->add_ropes(3);
                }

                registry.destroy(owner);
            }
        }
    };
}

entt::entity prefabs::RopePile::create()
{
    return create(0, 0);
}

entt::entity prefabs::RopePile::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 16.0f / 16.0f;
    const float height = 10.0f / 16.0f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::ROPE_PILE);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.02f);
    physics.set_bounciness(0.25f);

    ItemComponent item(ItemType::ROPE_PILE, ItemApplication::THROWABLE, ItemSlot::ACTIVE);
    item.set_weight(5);
    item.set_carrying_offset({0.0f, -0.3f});

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<RopePileScript>());
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<OpenableComponent>(entity);
    registry.emplace<GiveProjectileDamageComponent>(entity, 1);

    return entity;
}
