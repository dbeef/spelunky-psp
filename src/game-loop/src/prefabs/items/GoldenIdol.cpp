#include "prefabs/items/GoldenIdol.hpp"
#include "prefabs/particles/BigCollectParticle.hpp"

#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "audio/Audio.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "Level.hpp"
#include "other/Inventory.hpp"

namespace
{
    class GoldenIdolScript final : public ScriptBase
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto &registry = EntityRegistry::instance().get_registry();
            auto &item = registry.get<ItemComponent>(owner);

            if (item.is_carried())
            {
                // TODO: Spawn boulder here. Alternatively, make GameLoop/SpecialEventsSystem/CollectibleSystem
                //       keep track of golden idols, decoupling boulder logics from the idol itself.

                auto& physics = registry.get<PhysicsComponent>(owner);
                auto& position = registry.get<PositionComponent>(owner);
                auto carrier_entity = item.get_item_carrier_entity();
                auto& carrier = registry.get<ItemCarrierComponent>(carrier_entity);

                auto& tile_batch = Level::instance().get_tile_batch();
                MapTile* exit = nullptr;
                tile_batch.get_first_tile_of_given_type(MapTileType::EXIT, exit);
                assert(exit);

                ZoneComponent exit_zone(MapTile::PHYSICAL_WIDTH, MapTile::PHYSICAL_HEIGHT);
                PositionComponent exit_position(exit->get_center_x(), exit->get_center_y());

                if (physics.is_collision(exit_zone, exit_position, position))
                {
                    if (carrier.get_item(ItemType::WALLET) != entt::null)
                    {
                        Inventory::instance().add_dollars(5000);
                    }

                    prefabs::BigCollectParticle::create(position.x_center, position.y_center);
                    carrier.put_down_active_item();
                    Audio::instance().play(SFXType::COIN);
                    registry.destroy(owner);
                }
            }
        }
    };
}

entt::entity prefabs::GoldenIdol::create()
{
    return create(0, 0);
}

entt::entity prefabs::GoldenIdol::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::GOLDEN_IDOL);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.02f);

    ItemComponent item(ItemType::GOLDEN_IDOL, ItemApplication::THROWABLE, ItemSlot::ACTIVE);
    item.set_weight(7);
    item.set_carrying_offset({0.0f, -1.0f/8.0f});

    auto golden_idol_script = std::make_shared<GoldenIdolScript>();
    ScriptingComponent script(golden_idol_script);

    GiveProjectileDamageComponent give_projectile_damage(2);
    give_projectile_damage.set_mutual(true);

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, golden_idol_script);
    registry.emplace<GiveProjectileDamageComponent>(entity, give_projectile_damage);
    registry.emplace<HorizontalOrientationComponent>(entity);

    return entity;
}
