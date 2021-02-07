#include "prefabs/items/Bomb.hpp"
#include "prefabs/effects/Explosion.hpp"
#include "prefabs/particles/FlameParticle.hpp"

#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/ActivableComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"
#include "Level.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

namespace
{
    constexpr uint32_t fast_blinking_ms = 2500;
    constexpr uint32_t explosion_ms = 3500;

    class BombScript final : public ScriptBase
    {
    public:

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& activable = registry.get<ActivableComponent>(owner);
            auto& item = registry.get<ItemComponent>(owner);

            if (_armed)
            {
                item.set_type(ItemType::THROWABLE);
                _armed_timer_ms += delta_time_ms;
                if (_armed_timer_ms >= explosion_ms)
                {
                    explode(owner);

                    if (item.is_carried())
                    {
                        auto& item_carrier = item.get_item_carrier();
                        item_carrier.put_down_active_item();
                    }

                    registry.destroy(owner);
                    return;
                }
                else if (_armed_timer_ms >= fast_blinking_ms)
                {
                    auto& animation = registry.get<AnimationComponent>(owner);
                    animation.set_time_per_frame_ms(125);
                }
            }

            if (activable.activated && !_armed)
            {
                auto& quad = registry.get<QuadComponent>(owner);
                quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::BOMB_ARMED);

                AnimationComponent animation;
                animation.start(static_cast<std::size_t>(CollectiblesSpritesheetFrames::BOMB),
                                static_cast<std::size_t>(CollectiblesSpritesheetFrames::BOMB_ARMED),
                                250, true);
                registry.emplace<AnimationComponent>(owner, animation);

                _armed = true;
            }
        }
    private:

        uint32_t _armed_timer_ms = 0;
        bool _armed = false;

         static void explode(entt::entity owner)
         {
             // TODO: Play SFX
             // TODO: Shake camera

             auto& registry = EntityRegistry::instance().get_registry();

             auto& position = registry.get<PositionComponent>(owner);
             prefabs::Explosion::create(position.x_center, position.y_center);

             for (std::size_t index = 0; index < 4; index++)
             {
                 auto particle = prefabs::FlameParticle::create(position.x_center, position.y_center);
                 auto& physics = registry.get<PhysicsComponent>(particle);

                 float v_x = static_cast<float>(std::rand() % 2) / 15.0f;
                 float v_y = static_cast<float>(std::rand() % 2) / 10.0f;

                 if (std::rand() % 2)
                 {
                     v_x += 0.1f;
                 }
                 else
                 {
                     v_x -= 0.1f;
                 }

                 physics.set_velocity(v_x, v_y);
             }

             auto& tile_batch = Level::instance().get_tile_batch();
             MapTile *neighbours[9] = {nullptr};
             Level::instance().get_tile_batch().get_neighbouring_tiles(position.x_center, position.y_center, neighbours);

             for (auto tile : neighbours)
             {
                 if (tile && tile->destroyable)
                 {
                     // TODO: Spawn rubble particles on each destroyed tile.
                     tile->collidable = false;
                     tile->map_tile_type = MapTileType::NOTHING;
                 }
             }

             tile_batch.generate_cave_background();
             tile_batch.batch_vertices();
         }
    };
}

entt::entity prefabs::Bomb::create()
{
    return create(0, 0);
}

entt::entity prefabs::Bomb::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::BOMB);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.01f);
    physics.set_bounciness(0.35f);

    ItemComponent item(ItemType::ACTIVABLE, ItemSlot::ACTIVE);
    item.set_weight(2);
    item.set_carrying_offset({0.1f, 0.0f});

    ActivableComponent activable;
    activable.activate_combination = { InputEvent::THROWING_PRESSED };

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_2_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<BombScript>());
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<ActivableComponent>(entity, activable);

    return entity;
}
