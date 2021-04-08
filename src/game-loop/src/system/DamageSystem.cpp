#include "system/DamageSystem.hpp"
#include "EntityRegistry.hpp"
#include "audio/Audio.hpp"
#include "components/damage/TakeFallDamageComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/NpcTypeComponent.hpp"
#include "components/generic/ZoneComponent.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeJumpOnTopDamage.hpp"
#include "components/damage/GiveJumpOnTopDamage.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/TakeTileCollisionDamageComponent.hpp"
#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"
#include "components/damage/GiveMeleeDamageComponent.hpp"
#include "components/damage/TakeNpcTouchDamageComponent.hpp"
#include "components/damage/GiveNpcTouchDamageComponent.hpp"
#include "components/damage/GiveExplosionDamageComponent.hpp"
#include "components/damage/TakeExplosionDamageComponent.hpp"

#include <cmath>

namespace
{
    void remove_hitpoints(Hitpoint_t amount, entt::entity entity)
    {
        auto& registry = EntityRegistry::instance().get_registry();
        auto& hitpoints = registry.get<HitpointComponent>(entity);

        const NpcType npc_type = registry.has<NpcTypeComponent>(entity) ?
                                 registry.get<NpcTypeComponent>(entity).npc_type : NpcType::NONE;

        hitpoints.remove_hitpoints(amount);

        if (hitpoints.get_hitpoints() <= 0)
        {
            hitpoints.notify({npc_type});
            registry.destroy(entity);
        }
    }
}

void DamageSystem::update(std::uint32_t delta_time_ms)
{
    update_tile_collision_damage();
    update_falling_damage();
    update_projectile_damage();
    update_melee_damage();
    update_jump_on_top_damage();
    update_npc_touch_damage(delta_time_ms);
    update_explosion_damage();
}

void DamageSystem::update_melee_damage()
{
    auto &registry = EntityRegistry::instance().get_registry();
    auto bodies = registry.view<TakeMeleeDamageComponent, HitpointComponent, PhysicsComponent, PositionComponent>();
    auto melee_bodies = registry.view<GiveMeleeDamageComponent, PhysicsComponent, PositionComponent>();

    auto give_melee_damage = [&bodies, &registry](GiveMeleeDamageComponent& give_damage,
                                            PhysicsComponent& projectile_physics,
                                            PositionComponent& projectile_position)
    {
        bodies.each([&](
                entt::entity take_damage_entity,
                TakeMeleeDamageComponent& take_damage,
                HitpointComponent& hitpoints,
                PhysicsComponent& body_physics,
                PositionComponent& body_position)
        {
            if (!body_physics.is_collision(projectile_physics, projectile_position, body_position))
            {
                return;
            }

            const MeleeDamage_t damage = give_damage.get_damage();
            take_damage.notify(damage);
            remove_hitpoints(damage, take_damage_entity);
        });
    };

    registry.view<GiveMeleeDamageComponent, PhysicsComponent, PositionComponent>().each(give_melee_damage);
}

void DamageSystem::update_projectile_damage()
{
    auto &registry = EntityRegistry::instance().get_registry();
    auto bodies = registry.view<TakeProjectileDamageComponent, HitpointComponent, PhysicsComponent, PositionComponent>();
    auto projectiles = registry.view<GiveProjectileDamageComponent, PhysicsComponent, PositionComponent>();

    auto give_projectile_damage = [&bodies, &registry](entt::entity give_damage_entity,
                                                       GiveProjectileDamageComponent& give_damage,
                                                       PhysicsComponent& projectile_physics,
                                                       PositionComponent& projectile_position)
    {
        if (projectile_physics.get_x_velocity() == 0.0f && projectile_physics.get_y_velocity() == 0.0f)
        {
            return;
        }

        bodies.each([&](entt::entity take_damage_entity,
                             TakeProjectileDamageComponent& take_damage,
                             HitpointComponent& hitpoints,
                             PhysicsComponent& body_physics,
                             PositionComponent& body_position)
        {
            if (!registry.valid(give_damage_entity))
            {
                // May become invalid if it gets destroyed by observer.
                return;
            }

            if (!body_physics.is_collision(projectile_physics, projectile_position, body_position))
            {
                return;
            }

            if (take_damage_entity == give_damage_entity)
            {
                return;
            }

            const ProjectileDamage_t damage = give_damage.get_damage();

            take_damage.notify(damage);
            remove_hitpoints(damage, take_damage_entity);
            Audio::instance().play(SFXType::HIT);

            if (give_damage.is_mutual())
            {
                if (registry.has<HitpointComponent>(give_damage_entity))
                {
                    remove_hitpoints(damage, give_damage_entity);
                }

                if (!registry.valid(give_damage_entity))
                {
                    // May become invalid if it gets destroyed by observer.
                    return;
                }
                else
                {
                    // Update reference as may have become invalidated on registry resize:
                    auto& give_damage = registry.get<GiveProjectileDamageComponent>(give_damage_entity);
                    give_damage.notify(damage);
                }
            }
        });
    };

    registry.view<GiveProjectileDamageComponent, PhysicsComponent, PositionComponent>().each(give_projectile_damage);
}

void DamageSystem::update_falling_damage()
{
    auto &registry = EntityRegistry::instance().get_registry();
    auto bodies = registry.view<TakeFallDamageComponent, HitpointComponent, PhysicsComponent>();

    auto give_fall_damage = [](TakeFallDamageComponent& fall_damage, HitpointComponent& hitpoints, PhysicsComponent& physics)
    {
        const bool damage_taken = fall_damage.update(physics);
        if (damage_taken)
        {
            const FallDamage_t damage = fall_damage.get_falling_damage();

            fall_damage.notify(damage);
            hitpoints.remove_hitpoints(damage);
            Audio::instance().play(SFXType::HIT);

            if (hitpoints.get_hitpoints() <= 0)
            {
                hitpoints.notify({});
            }
        }
    };

    registry.view<TakeFallDamageComponent, HitpointComponent, PhysicsComponent>().each(give_fall_damage);
}

void DamageSystem::update_tile_collision_damage()
{
    auto &registry = EntityRegistry::instance().get_registry();
    auto bodies = registry.view<TakeFallDamageComponent, HitpointComponent, PhysicsComponent>();

    auto give_tile_collision_damage = [&registry](entt::entity tile_collision_entity,
                                         TakeTileCollisionDamageComponent& tile_collision_damage,
                                         HitpointComponent& hitpoints,
                                         PhysicsComponent& physics)
    {
        const bool damage_taken = tile_collision_damage.update(physics);
        if (damage_taken)
        {
            const TileCollisionDamage_t damage = tile_collision_damage.get_tile_collision_damage();
            remove_hitpoints(damage, tile_collision_entity);
            Audio::instance().play(SFXType::HIT);
        }
    };

    registry.view<TakeTileCollisionDamageComponent, HitpointComponent, PhysicsComponent>().each(give_tile_collision_damage);
}

void DamageSystem::update_jump_on_top_damage()
{
    auto &registry = EntityRegistry::instance().get_registry();
    auto bodies = registry.view<TakeJumpOnTopDamageComponent, HitpointComponent, PhysicsComponent, PositionComponent>();
    auto jumping_bodies = registry.view<GiveJumpOnTopDamageComponent, PhysicsComponent, PositionComponent>();

    auto give_jump_on_top_damage = [&bodies, &registry](entt::entity give_damage_entity,
                                                        GiveJumpOnTopDamageComponent& give_damage,
                                                        PhysicsComponent& jumping_physics,
                                                        PositionComponent& jumping_position)
    {
        if (jumping_physics.get_y_velocity() <= 0.0f)
        {
            return;
        }

        bodies.each([&](entt::entity take_damage_entity,
                             TakeJumpOnTopDamageComponent& take_damage,
                             HitpointComponent& hitpoints,
                             PhysicsComponent& body_physics,
                             PositionComponent& body_position)
        {
            if (jumping_physics.get_y_velocity() <= 0.0f)
            {
                return;
            }

            if (!body_physics.is_collision(jumping_physics, jumping_position, body_position))
            {
                return;
            }

            if (take_damage_entity == give_damage_entity)
            {
                return;
            }

            if (jumping_position.y_center > body_position.y_center - (body_physics.get_height() / 2.0f))
            {
                return;
            }

            jumping_physics.set_y_velocity(-0.15f);

            const JumpOnTopDamage_t damage = give_damage.get_damage();
            take_damage.notify(damage);
            remove_hitpoints(damage, take_damage_entity);
            Audio::instance().play(SFXType::HIT);
        });
    };

    registry.view<GiveJumpOnTopDamageComponent, PhysicsComponent, PositionComponent>().each(give_jump_on_top_damage);
}

void DamageSystem::update_explosion_damage()
{
    auto &registry = EntityRegistry::instance().get_registry();

    auto bodies = registry.view<PhysicsComponent, PositionComponent>();
    auto bodies_giving_damage = registry.view<GiveExplosionDamageComponent, ZoneComponent, PositionComponent>();

    auto give_explosion_damage = [&](entt::entity give_damage_entity,
                                     GiveExplosionDamageComponent& give_damage,
                                     ZoneComponent& give_damage_zone,
                                     PositionComponent& give_damage_position)
    {
        bodies.each([&](entt::entity body_entity,
                             PhysicsComponent& body_physics,
                             PositionComponent& body_position)
        {

            if (body_physics.is_collision(give_damage_zone, give_damage_position, body_position))
            {
                // Calculate direction vector and set velocity outwards explosion:
                float x_direction = give_damage_position.x_center - body_position.x_center;
                float y_direction = give_damage_position.y_center - body_position.y_center;

                const float x_velocity = -copysign(0.1f, x_direction);
                const float y_velocity = -copysign(0.1f, y_direction);

                body_physics.set_x_velocity(x_velocity);
                body_physics.set_y_velocity(y_velocity);

                // Give damage if supported:
                if (registry.has<TakeExplosionDamageComponent>(body_entity))
                {
                    auto& take_damage_hitpoints = registry.get<HitpointComponent>(body_entity);
                    auto& take_damage_explosion = registry.get<TakeExplosionDamageComponent>(body_entity);

                    take_damage_hitpoints.remove_hitpoints(take_damage_hitpoints.get_hitpoints() + 1);
                    take_damage_hitpoints.notify({});
                    take_damage_explosion.notify({});

                    // Only remove if it was an NPC:
                    if (registry.has<NpcTypeComponent>(body_entity))
                    {
                        // FIXME: What if it is i.e caveman? Cavemen don't disappear upon death - should implement a
                        //        cleanup method that would be called in the end of DamageSystem update method, plus
                        //        additional flag indicating that entity can be disposed.
                        registry.destroy(body_entity);
                    }
                }
            }
        });
    };

    bodies_giving_damage.each(give_explosion_damage);
}

void DamageSystem::update_npc_touch_damage(std::uint32_t delta_time_ms)
{
    const NpcDamage_t default_npc_touch_damage = 1;

    auto &registry = EntityRegistry::instance().get_registry();

    auto bodies_taking_damage = registry.view<TakeNpcTouchDamageComponent, HitpointComponent, PhysicsComponent, PositionComponent>();
    auto bodies_giving_damage = registry.view<GiveNpcTouchDamageComponent, PhysicsComponent, PositionComponent>();

    auto give_tile_collision_damage = [&](entt::entity take_damage_entity,
                                          TakeNpcTouchDamageComponent& take_damage,
                                          HitpointComponent& take_damage_hitpoints,
                                          PhysicsComponent& take_damage_physics,
                                          PositionComponent& take_damage_position)
    {
        bodies_giving_damage.each([&](GiveNpcTouchDamageComponent& give_damage,
                                           PhysicsComponent& give_damage_physics,
                                           PositionComponent& give_damage_position)
        {
            if (give_damage.cooldown_ms > 0)
            {
                give_damage.cooldown_ms -= delta_time_ms;
                return;
            }

            if (take_damage_physics.is_collision(give_damage_physics, give_damage_position, take_damage_position))
            {
                take_damage_hitpoints.remove_hitpoints(default_npc_touch_damage);
                take_damage.notify(default_npc_touch_damage);
                give_damage.cooldown_ms = GiveNpcTouchDamageComponent::TOP_COOLDOWN_MS;

                if (take_damage_hitpoints.get_hitpoints() <= 0)
                {
                    take_damage_hitpoints.notify({});
                }
            }
        });
    };

    // FIXME: This should be reversed - bodies giving damage looking for bodies taking damage
    registry.view<TakeNpcTouchDamageComponent, HitpointComponent, PhysicsComponent, PositionComponent>().each(give_tile_collision_damage);
}
