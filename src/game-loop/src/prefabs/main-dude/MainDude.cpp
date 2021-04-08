#include "prefabs/main-dude/MainDude.hpp"
#include "prefabs/items/BombSpawner.hpp"
#include "prefabs/items/RopeSpawner.hpp"
#include "prefabs/items/Whip.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/ClimbingComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/InputComponent.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeNpcTouchDamageComponent.hpp"
#include "components/damage/GiveJumpOnTopDamage.hpp"
#include "components/damage/TakeExplosionDamageComponent.hpp"
#include "components/damage/TakeFallDamageComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/specialized/MainDudeComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "other/Inventory.hpp"

namespace
{
    static constexpr float DEFAULT_MAX_Y_VELOCITY = 0.39f;
}

namespace prefabs
{
    entt::entity MainDude::create()
    {
        return create(0, 0);
    }

    entt::entity MainDude::create(float pos_x_center, float pos_y_center)
    {
        auto &registry = EntityRegistry::instance().get_registry();

        const auto entity = registry.create();

        PhysicsComponent physics(1 - (2.0f / 16.0f), 1 - (1.0f / 16.0f));

        PositionComponent position(pos_x_center, pos_y_center);
        QuadComponent quad(TextureType::MAIN_DUDE, 1, 1);
        AnimationComponent animation;
        InputComponent input;
        ItemCarrierComponent item_carrier;

        MeshComponent mesh;
        mesh.camera_type = CameraType::MODEL_VIEW_SPACE;
        mesh.rendering_layer = RenderingLayer::LAYER_3_DUDE;

        registry.emplace<PositionComponent>(entity, position);
        registry.emplace<QuadComponent>(entity, quad);
        registry.emplace<AnimationComponent>(entity, animation);
        registry.emplace<PhysicsComponent>(entity, physics);
        registry.emplace<MeshComponent>(entity, mesh);
        registry.emplace<InputComponent>(entity, input);
        registry.emplace<HorizontalOrientationComponent>(entity);
        registry.emplace<ItemCarrierComponent>(entity, item_carrier);
        registry.emplace<GiveJumpOnTopDamageComponent>(entity, 1);

        // FIXME: How to differentiate between projectiles that the main dude has just thrown,
        //        and the ones from outside, i.e from arrow trap?
        //
        //        a) add offset immediately after throwing a projectile so there would be no collision
        //        b) when checking collisions, ignore projectiles moving away?
        //        c) give a high velocity treshold?
        //        d) carry some indicator (i.e component) that would store source of projectile (i.e arrow trop, main dude, nothing)
        //
        //        b) save entity id of thrown projectile and ignore it when checking collision
        //        c) short cooldown just after throwing something?

        // Initialization order is important in this case - MainDudeComponent must be the last to create.
        MainDudeComponent main_dude(entity);
        registry.emplace<MainDudeComponent>(entity, main_dude);

        HitpointComponent hitpoints(Inventory::instance().get_hearts());
        hitpoints.add_observer(reinterpret_cast<Observer<DeathEvent> *>(main_dude.get_death_observer()));
        registry.emplace<HitpointComponent>(entity, hitpoints);

        ClimbingComponent climbing;
        climbing.add_observer(reinterpret_cast<Observer<ClimbingEvent> *>(main_dude.get_climbing_observer()));
        registry.emplace<ClimbingComponent>(entity, climbing);

        TakeFallDamageComponent take_fall_damage(1, DEFAULT_MAX_Y_VELOCITY);
        take_fall_damage.add_observer(reinterpret_cast<Observer<FallDamage_t> *>(main_dude.get_fall_observer()));
        registry.emplace<TakeFallDamageComponent>(entity, take_fall_damage);

        TakeNpcTouchDamageComponent take_npc_damage;
        take_npc_damage.add_observer(reinterpret_cast<Observer<NpcDamage_t> *>(main_dude.get_npc_damage_observer()));
        registry.emplace<TakeNpcTouchDamageComponent>(entity, take_npc_damage);

        TakeExplosionDamageComponent take_explosion_damage;
        take_explosion_damage.add_observer(reinterpret_cast<Observer<ExplosionDamageTakenEvent> *>(main_dude.get_explosion_damage_observer()));
        registry.emplace<TakeExplosionDamageComponent>(entity, take_explosion_damage);

        TakeProjectileDamageComponent take_projectile_damage;
        take_projectile_damage.add_observer(reinterpret_cast<Observer<ProjectileDamage_t> *>(main_dude.get_projectile_damage_observer()));
        registry.emplace<TakeProjectileDamageComponent>(entity, take_projectile_damage);

        {
            auto& carrier = registry.get<ItemCarrierComponent>(entity);

            carrier.pick_up_item(prefabs::BombSpawner::create(), entity);
            carrier.pick_up_item(prefabs::RopeSpawner::create(), entity);
            carrier.pick_up_item(prefabs::Whip::create(pos_x_center, pos_y_center), entity);
        }

        return entity;
    }
}
