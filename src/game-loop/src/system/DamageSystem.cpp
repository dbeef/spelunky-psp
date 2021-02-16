#include "system/DamageSystem.hpp"
#include "components/damage/TakeFallDamageComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeJumpOnTopDamage.hpp"
#include "components/damage/GiveJumpOnTopDamage.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/TakeTileCollisionDamageComponent.hpp"
#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "components/damage/TakeMeleeDamageComponent.hpp"
#include "components/damage/GiveMeleeDamageComponent.hpp"
#include "EntityRegistry.hpp"
#include "audio/Audio.hpp"

namespace
{
    void remove_hitpoints(HitpointComponent& hitpoints, Hitpoint_t amount)
    {
        hitpoints.remove_hitpoints(amount);

        if (hitpoints.get_hitpoints() <= 0)
        {
            hitpoints.notify({});
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
}

void DamageSystem::update_melee_damage()
{
    auto &registry = EntityRegistry::instance().get_registry();
    auto bodies = registry.view<TakeMeleeDamageComponent, HitpointComponent, PhysicsComponent, PositionComponent>();
    auto melee_bodies = registry.view<GiveMeleeDamageComponent, PhysicsComponent, PositionComponent>();

    auto give_melee_damage = [&bodies](GiveMeleeDamageComponent& give_damage,
                                            PhysicsComponent& projectile_physics,
                                            PositionComponent& projectile_position)
    {
        bodies.each([&projectile_physics, &projectile_position, &give_damage](
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
            hitpoints.remove_hitpoints(damage);

            if (hitpoints.get_hitpoints() <= 0)
            {
                hitpoints.notify({});
            }
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
            remove_hitpoints(hitpoints, damage);

            if (give_damage.is_mutual())
            {
                if (registry.has<HitpointComponent>(give_damage_entity))
                {
                    remove_hitpoints(registry.get<HitpointComponent>(give_damage_entity), damage);
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

    auto give_tile_collision_damage = [](TakeTileCollisionDamageComponent& tile_collision_damage, HitpointComponent& hitpoints, PhysicsComponent& physics)
    {
        const bool damage_taken = tile_collision_damage.update(physics);
        if (damage_taken)
        {
            const TileCollisionDamage_t damage = tile_collision_damage.get_tile_collision_damage();

            hitpoints.remove_hitpoints(damage);

            if (hitpoints.get_hitpoints() <= 0)
            {
                hitpoints.notify({});
            }
        }
    };

    registry.view<TakeTileCollisionDamageComponent, HitpointComponent, PhysicsComponent>().each(give_tile_collision_damage);
}

void DamageSystem::update_jump_on_top_damage()
{
    auto &registry = EntityRegistry::instance().get_registry();
    auto bodies = registry.view<TakeJumpOnTopDamageComponent, HitpointComponent, PhysicsComponent, PositionComponent>();
    auto jumping_bodies = registry.view<GiveJumpOnTopDamageComponent, PhysicsComponent, PositionComponent>();

    auto give_jump_on_top_damage = [&bodies](entt::entity give_damage_entity,
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
            remove_hitpoints(hitpoints, damage);
        });
    };

    registry.view<GiveJumpOnTopDamageComponent, PhysicsComponent, PositionComponent>().each(give_jump_on_top_damage);
}
