#include "prefabs/other/Bullet.hpp"
#include "prefabs/particles/SmokePuffParticle.hpp"

#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeTileCollisionDamageComponent.hpp"
#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

namespace
{
    class BulletDeathObserver final : public Observer<DeathEvent>
    {
    public:

        explicit BulletDeathObserver(entt::entity bullet) :_bullet(bullet) {}

        void on_notify(const DeathEvent*) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            auto& position = registry.get<PositionComponent>(_bullet);

            prefabs::SmokePuffParticle::create(position.x_center, position.y_center);
        }

    private:
        const entt::entity _bullet;
    };

    class BulletScript final : public ScriptBase
    {
    public:
        explicit BulletScript(entt::entity bullet) : _death_observer(bullet) {}

        BulletDeathObserver* get_observer()
        {
            return &_death_observer;
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
        }
    private:
        BulletDeathObserver _death_observer;
    };
}

entt::entity prefabs::Bullet::create()
{
    return create(0, 0);
}

entt::entity prefabs::Bullet::create(float pos_x_center, float pos_y_center, entt::entity projectile_source)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 0.5f;
    const float height = 0.5f;

    PositionComponent position(pos_x_center, pos_y_center);
    QuadComponent quad(TextureType::COLLECTIBLES, width, height);
    MeshComponent mesh;
    PhysicsComponent physics(width, height);

    physics.disable_gravity();

    quad.frame_changed(CollectiblesSpritesheetFrames::BULLET);
    mesh.rendering_layer = RenderingLayer::LAYER_3_ITEMS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    auto bullet_script = std::make_shared<BulletScript>(entity);
    ScriptingComponent script(bullet_script);

    HitpointComponent hitpoints(1, true);
    hitpoints.add_observer(reinterpret_cast<Observer<DeathEvent>*>(bullet_script->get_observer()));

    GiveProjectileDamageComponent give_projectile_damage(2, projectile_source);
    give_projectile_damage.set_mutual(true);

    TakeTileCollisionDamageComponent tile_collision_damage(1);

    registry.emplace<PositionComponent>(entity, position);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);
    registry.emplace<PhysicsComponent>(entity, physics);
    registry.emplace<HorizontalOrientationComponent>(entity);
    registry.emplace<ScriptingComponent>(entity, script);
    registry.emplace<GiveProjectileDamageComponent>(entity, give_projectile_damage);
    registry.emplace<TakeTileCollisionDamageComponent>(entity, tile_collision_damage);
    registry.emplace<HitpointComponent>(entity, hitpoints);

    return entity;
}
