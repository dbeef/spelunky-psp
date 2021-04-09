#include "prefabs/items/Skull.hpp"
#include "prefabs/particles/SmokePuffParticle.hpp"
#include "prefabs/particles/BonesParticle.hpp"

#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"
#include "components/damage/TakeTileCollisionDamageComponent.hpp"
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
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

namespace
{
    class SkullDeathObserver final : public Observer<DeathEvent>
    {
    public:

        explicit SkullDeathObserver(entt::entity skull) : _skull(skull) {}

        void on_notify(const DeathEvent*) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            const auto& position = registry.get<PositionComponent>(_skull);

            prefabs::SmokePuffParticle::create(position.x_center, position.y_center);

            const auto bones_particle = prefabs::BonesParticle::create(position.x_center, position.y_center);
            auto& bones_particle_physics = registry.get<PhysicsComponent>(bones_particle);

            bones_particle_physics.set_velocity(0.1f, -0.1f);

            auto& hitpoints = registry.get<HitpointComponent>(_skull);
            hitpoints.remove_observer(this);

            auto& item = registry.get<ItemComponent>(_skull);
            if (item.is_carried())
            {
                auto& item_carrier = item.get_item_carrier();
                item_carrier.put_down_active_item();
            }
        }

    private:
        const entt::entity _skull;
    };
    
    class SkullScript final : public ScriptBase
    {
    public:
        explicit SkullScript(entt::entity skull) : _death_observer(skull) {}

        SkullDeathObserver* get_observer()
        {
            return &_death_observer;
        }
        
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
        }

    private:
        SkullDeathObserver _death_observer;
    };
}

entt::entity prefabs::Skull::create()
{
    return create(0, 0);
}

entt::entity prefabs::Skull::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed(NPCSpritesheetFrames::SKULL);

    PhysicsComponent physics(width, height);
    physics.set_friction(0.02f);

    ItemComponent item(ItemType::SKULL, ItemApplication::THROWABLE, ItemSlot::ACTIVE);
    item.set_weight(2);
    item.set_carrying_offset({0.0f, -1.0f/8.0f});

    auto skull_script = std::make_shared<SkullScript>(entity);
    ScriptingComponent script(skull_script);

    HitpointComponent hitpoints(1, true);
    hitpoints.add_observer(reinterpret_cast<Observer<DeathEvent>*>(skull_script->get_observer()));

    GiveProjectileDamageComponent give_projectile_damage(1);
    give_projectile_damage.set_mutual(true);

    float critical_speed_x = 0.1f;
    float critical_speed_y = 0.1f;
    TakeTileCollisionDamageComponent tile_collision_damage(1, critical_speed_x, critical_speed_y);

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<ScriptingComponent>(entity, script);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_3_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<ItemComponent>(entity, item);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<GiveProjectileDamageComponent>(entity, give_projectile_damage);
    registry.emplace<TakeProjectileDamageComponent>(entity);
    registry.emplace<TakeTileCollisionDamageComponent>(entity, tile_collision_damage);
    registry.emplace<HitpointComponent>(entity, hitpoints);
    registry.emplace<TakeMeleeDamageComponent>(entity);

    return entity;
}
