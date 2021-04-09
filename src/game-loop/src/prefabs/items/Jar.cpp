#include "prefabs/items/Jar.hpp"
#include "prefabs/particles/SmokePuffParticle.hpp"
#include "prefabs/particles/RubbleSmallParticle.hpp"
#include "prefabs/collectibles/BigGem.hpp"
#include "prefabs/collectibles/GoldChunk.hpp"
#include "prefabs/collectibles/GoldNugget.hpp"
#include "prefabs/npc/Spider.hpp"
#include "prefabs/npc/Snake.hpp"

#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeTileCollisionDamageComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"
#include "components/generic/ItemComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

#include <cmath>

namespace
{
    enum class JarLootType : uint8_t
    {
        BIG_GEM,
        GOLD_CHUNK,
        GOLD_NUGGET,
        SPIDER,
        SNAKE,
        _SIZE
    };

    JarLootType get_random_loot()
    {
        return static_cast<JarLootType>(std::rand() % static_cast<int>(JarLootType::_SIZE));
    }

    class JarDeathObserver final : public Observer<DeathEvent>
    {
    public:

        explicit JarDeathObserver(entt::entity jar) : _jar(jar) {}

        void on_notify(const DeathEvent*) override
        {
            auto& registry = EntityRegistry::instance().get_registry();

            auto& position = registry.get<PositionComponent>(_jar);
            prefabs::SmokePuffParticle::create(position.x_center, position.y_center);

            switch (get_random_loot())
            {
                case JarLootType::BIG_GEM: prefabs::BigGem::create(position.x_center, position.y_center); break;
                case JarLootType::GOLD_CHUNK: prefabs::GoldChunk::create(position.x_center, position.y_center); break;
                case JarLootType::GOLD_NUGGET: prefabs::GoldNugget::create(position.x_center, position.y_center); break;
                case JarLootType::SPIDER: prefabs::Spider::create(position.x_center, position.y_center, true); break;
                case JarLootType::SNAKE: prefabs::Snake::create(position.x_center, position.y_center); break;
                default: assert(false);
            }

            for (int index = 0; index < 5; ++index)
            {
                auto rubble = prefabs::RubbleSmallParticle::create(position.x_center, position.y_center);
                auto& rubble_physics = registry.get<PhysicsComponent>(rubble);

                float rubble_velocity_x = 0;
                float rubble_velocity_y = 0;

                rubble_velocity_x = static_cast<float>(std::rand() % 10) / 100.0f;
                rubble_velocity_y = static_cast<float>(std::rand() % 10) / 100.0f;

                rubble_physics.set_x_velocity(rubble_velocity_x);
                rubble_physics.set_y_velocity(rubble_velocity_y);
            }

            auto& hitpoints = registry.get<HitpointComponent>(_jar);
            hitpoints.remove_observer(this);

            auto& item = registry.get<ItemComponent>(_jar);
            if (item.is_carried())
            {
                auto& item_carrier = item.get_item_carrier();
                item_carrier.put_down_active_item();
            }
        }

    private:
        const entt::entity _jar;
    };

    class JarScript final : public ScriptBase
    {
    public:

        explicit JarScript(entt::entity jar) : _death_observer(jar) {}

        JarDeathObserver* get_observer()
        {
            return &_death_observer;
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
        }

    private:
        JarDeathObserver _death_observer;
    };
}

entt::entity prefabs::Jar::create()
{
    return create(0, 0);
}

entt::entity prefabs::Jar::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    quad.frame_changed(CollectiblesSpritesheetFrames::JAR);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.02f);

    ItemComponent item(ItemType::JAR, ItemApplication::THROWABLE, ItemSlot::ACTIVE);
    item.set_weight(2);
    item.set_carrying_offset({0.0f, -1.0f/8.0f});

    auto jar_script = std::make_shared<JarScript>(entity);
    ScriptingComponent script(jar_script);

    GiveProjectileDamageComponent give_projectile_damage(2);
    give_projectile_damage.set_mutual(true);

    HitpointComponent hitpoints(1, true);
    hitpoints.add_observer(reinterpret_cast<Observer<DeathEvent>*>(jar_script->get_observer()));

    float critical_speed_x = 0.1f;
    float critical_speed_y = 0.1f;
    TakeTileCollisionDamageComponent tile_collision_damage(1, critical_speed_x, critical_speed_y);

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<ScriptingComponent>(entity, jar_script);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<TakeMeleeDamageComponent>(entity);
    registry.emplace<TakeProjectileDamageComponent>(entity);
    registry.emplace<GiveProjectileDamageComponent>(entity, give_projectile_damage);
    registry.emplace<TakeTileCollisionDamageComponent>(entity, tile_collision_damage);
    registry.emplace<HitpointComponent>(entity, hitpoints);

    return entity;
}
