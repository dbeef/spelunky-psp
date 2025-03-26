#include "prefabs/items/Crate.hpp"
#include "prefabs/items/Jetpack.hpp"
#include "prefabs/items/Cape.hpp"
#include "prefabs/items/Pistol.hpp"
#include "prefabs/items/Shotgun.hpp"
#include "prefabs/items/BombSpawner.hpp"
#include "prefabs/items/RopeSpawner.hpp"
#include "prefabs/items/SpikeShoes.hpp"
#include "prefabs/items/SpringShoes.hpp"
#include "prefabs/items/Mitt.hpp"
#include "prefabs/items/Glove.hpp"
#include "prefabs/items/Compass.hpp"
#include "prefabs/particles/PoofParticle.hpp"
#include "prefabs/particles/ItemCollectedParticle.hpp"
#include "prefabs/particles/RopeCollectedParticle.hpp"
#include "prefabs/particles/BombCollectedParticle.hpp"

#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/OpenableComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "audio/Audio.hpp"
#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

#include <cassert>

namespace
{
    enum class CrateLootType : uint8_t
    {
        ITEM = 0,
        ROPE,
        BOMBS,
        _SIZE
    };

    enum class CrateItemType : uint8_t
    {
        JETPACK = 0,
        CAPE,
        PISTOL,
        SHOTGUN,
        SPIKE_SHOES,
        SPRING_SHOES,
        MITT,
        GLOVE,
        COMPASS,
        _SIZE
    };

    CrateLootType get_random_loot()
    {
        return static_cast<CrateLootType>(std::rand() % static_cast<int>(CrateLootType::_SIZE));
    }

    CrateItemType get_random_item()
    {
        return static_cast<CrateItemType>(std::rand() % static_cast<int>(CrateItemType::_SIZE));
    }

    class CrateScript final : public ScriptBase
    {
    public:

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& openable = registry.get<OpenableComponent>(owner);

            if (!openable.opened)
            {
                return;
            }

            Audio::instance().play(SFXType::PICKUP);
            auto& position = registry.get<PositionComponent>(owner);

            switch (get_random_loot())
            {
                case CrateLootType::ITEM:
                {
                    switch (get_random_item())
                    {
                        case CrateItemType::JETPACK: prefabs::Jetpack::create(position.x_center, position.y_center); break;
                        case CrateItemType::CAPE: prefabs::Cape::create(position.x_center, position.y_center); break;
                        case CrateItemType::PISTOL: prefabs::Pistol::create(position.x_center, position.y_center); break;
                        case CrateItemType::SHOTGUN: prefabs::Shotgun::create(position.x_center, position.y_center); break;
                        case CrateItemType::SPIKE_SHOES: prefabs::SpikeShoes::create(position.x_center, position.y_center); break;
                        case CrateItemType::SPRING_SHOES: prefabs::SpringShoes::create(position.x_center, position.y_center); break;
                        case CrateItemType::MITT: prefabs::Mitt::create(position.x_center, position.y_center); break;
                        case CrateItemType::GLOVE: prefabs::Glove::create(position.x_center, position.y_center); break;
                        case CrateItemType::COMPASS: prefabs::Compass::create(position.x_center, position.y_center); break;
                    }

                    prefabs::ItemCollectedParticle::create(position.x_center, position.y_center - 1.0f);
                    break;
                }
                case CrateLootType::ROPE:
                {
                    auto& item_carrier_component = registry.get<ItemCarrierComponent>(openable.who_opened);
                    const auto spawner = item_carrier_component.get_item(ItemType::ROPE_SPAWNER);

                    if (spawner != entt::null)
                    {
                        auto& rope_spawner_script_component = registry.get<ScriptingComponent>(spawner);
                        auto* rope_spawner_script = rope_spawner_script_component.get<prefabs::RopeSpawner::RopeSpawnerScript>();
                        rope_spawner_script->add_ropes(4);
                    }

                    prefabs::RopeCollectedParticle::create(position.x_center, position.y_center - 1.0f);
                    break;
                }
                case CrateLootType::BOMBS:
                {
                    auto& item_carrier_component = registry.get<ItemCarrierComponent>(openable.who_opened);
                    const auto spawner = item_carrier_component.get_item(ItemType::BOMB_SPAWNER);

                    if (spawner != entt::null)
                    {
                        auto& bomb_spawner_script_component = registry.get<ScriptingComponent>(spawner);
                        auto* bomb_spawner_script = bomb_spawner_script_component.get<prefabs::BombSpawner::BombSpawnerScript>();
                        bomb_spawner_script->add_bombs(4);
                    }

                    prefabs::BombCollectedParticle::create(position.x_center, position.y_center - 1.0f);
                    break;
                }
                default: assert(false);
            }

            prefabs::PoofParticle::create(position.x_center, position.y_center);

            auto& item = registry.get<ItemComponent>(owner);
            if (item.is_carried())
            {
                auto& item_carrier = item.get_item_carrier();
                item_carrier.put_down_active_item();
            }

            registry.destroy(owner);
        }
    };
}

entt::entity prefabs::Crate::create()
{
    return create(0, 0);
}

entt::entity prefabs::Crate::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::CRATE);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.02f);
    physics.set_bounciness(0.25f);

    ItemComponent item(ItemType::CRATE, ItemApplication::OPENABLE, ItemSlot::ACTIVE);
    item.set_weight(5);
    item.set_carrying_offset({0.0f, -0.3f});

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<CrateScript>());
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<OpenableComponent>(entity);
    registry.emplace<GiveProjectileDamageComponent>(entity, 1);

    return entity;
}
