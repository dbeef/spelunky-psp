#include "system/DamageSystem.hpp"
#include "EntityRegistry.hpp"
#include "audio/Audio.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

#include "components/damage/TakeFallDamageComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/NpcTypeComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
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
#include "components/damage/GiveSpikesDamageComponent.hpp"
#include "components/damage/TakeSpikesDamageComponent.hpp"

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
            hitpoints.remove_all_observers();

            if (hitpoints.is_disposed_when_zero())
            {
                registry.destroy(entity);
            }
        }
    }
}

void DamageSystem::update(std::uint32_t delta_time_ms)
{
    update_tile_collision_damage();
    update_falling_damage();
    update_projectile_damage();
    update_melee_damage(delta_time_ms);
    update_jump_on_top_damage(delta_time_ms);
    update_npc_touch_damage(delta_time_ms);
    update_explosion_damage();
    update_spikes_damage();
}

void DamageSystem::update_melee_damage(std::uint32_t delta_time_ms)
{
    auto &registry = EntityRegistry::instance().get_registry();
    auto bodies = registry.view<TakeMeleeDamageComponent, HitpointComponent, PhysicsComponent, PositionComponent>();
    auto melee_bodies = registry.view<GiveMeleeDamageComponent, PhysicsComponent, PositionComponent>();

    auto give_melee_damage = [&](entt::entity give_damage_entity,
                                 GiveMeleeDamageComponent& give_damage,
                                 PhysicsComponent& projectile_physics,
                                 PositionComponent& projectile_position)
    {
        // Check if bodies from last update still do overlap:
        auto& last_update_overlapping_bodies = give_damage.get_last_update_overlaping_bodies();
        const auto it = std::remove_if(last_update_overlapping_bodies.begin(),
                                       last_update_overlapping_bodies.end(),
                                       [&](const entt::entity recently_overlapped_body)
        {
            if (!registry.valid(recently_overlapped_body))
            {
                return true;
            }

            auto& recent_body_physics = registry.get<PhysicsComponent>(recently_overlapped_body);
            auto& recent_body_position = registry.get<PositionComponent>(recently_overlapped_body);

            return !projectile_physics.is_collision(recent_body_physics, recent_body_position, projectile_position);
        });

        if  (it != last_update_overlapping_bodies.end())
        {
            last_update_overlapping_bodies.erase(it);
        }

        bodies.each([&](
                entt::entity take_damage_entity,
                TakeMeleeDamageComponent& take_damage,
                HitpointComponent& hitpoints,
                PhysicsComponent& body_physics,
                PositionComponent& body_position)
        {
            if (std::find(last_update_overlapping_bodies.begin(), last_update_overlapping_bodies.end(), take_damage_entity) != last_update_overlapping_bodies.end())
            {
                return;
            }

            if (!body_physics.is_collision(projectile_physics, projectile_position, body_position))
            {
                return;
            }

            last_update_overlapping_bodies.push_back(take_damage_entity);

            TakenMeleeDamageEvent event;
            event.amount = give_damage.get_damage();
            event.source = give_damage_entity;

            take_damage.notify(event);
            remove_hitpoints(event.amount, take_damage_entity);
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

        // Check if bodies from last update still do overlap:
        auto& last_update_overlapping_bodies = give_damage.get_last_update_overlaping_bodies();
        const auto it = std::remove_if(last_update_overlapping_bodies.begin(),
                       last_update_overlapping_bodies.end(),
                       [&](const entt::entity recently_overlapped_body)
        {
            if (!registry.valid(recently_overlapped_body))
            {
                return true;
            }

            auto& recent_body_physics = registry.get<PhysicsComponent>(recently_overlapped_body);
            auto& recent_body_position = registry.get<PositionComponent>(recently_overlapped_body);

            return !projectile_physics.is_collision(recent_body_physics, recent_body_position, projectile_position);
        });
        if  (it != last_update_overlapping_bodies.end())
        {
            last_update_overlapping_bodies.erase(it);
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

            if (give_damage.is_last_throw_source(take_damage_entity))
            {
                return;
            }

            if (std::find(last_update_overlapping_bodies.begin(), last_update_overlapping_bodies.end(), take_damage_entity) != last_update_overlapping_bodies.end())
            {
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

            last_update_overlapping_bodies.push_back(take_damage_entity);

            const ProjectileDamage_t damage = give_damage.get_damage();

            TakenProjectileDamageEvent event;
            event.amount = damage;
            event.source = give_damage.get_last_throw_source();

            take_damage.notify(event);
            remove_hitpoints(damage, take_damage_entity);
            Audio::instance().play(SFXType::HIT);

            // Set same horizontal direction as projectile:
            body_physics.set_x_velocity(0.5f * projectile_physics.get_x_velocity());
            // Make it point slightly upwards:
            body_physics.set_y_velocity(-0.1f);

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

    auto give_tile_collision_damage = [&](entt::entity tile_collision_entity,
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

void DamageSystem::update_jump_on_top_damage(uint32_t delta_time_ms)
{
    auto &registry = EntityRegistry::instance().get_registry();
    auto bodies = registry.view<TakeJumpOnTopDamageComponent, HitpointComponent, PhysicsComponent, PositionComponent>();
    auto jumping_bodies = registry.view<GiveJumpOnTopDamageComponent, PhysicsComponent, PositionComponent>();

    auto give_jump_on_top_damage = [&](entt::entity give_damage_entity,
                                       GiveJumpOnTopDamageComponent& give_damage,
                                       PhysicsComponent& jumping_physics,
                                       PositionComponent& jumping_position)
    {
        give_damage.update_cooldown(delta_time_ms);
        bool damage_given = false;

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

            JumpOnTopDamage_t damage = give_damage.get_damage();

            if (registry.has<ItemCarrierComponent>(give_damage_entity))
            {
                const auto item_carrier = registry.get<ItemCarrierComponent>(give_damage_entity);
                damage += item_carrier.get_modifiers().additional_jump_on_top_damage;
            }

            TakenJumpOnTopDamageEvent event;
            event.amount = damage;
            event.source = give_damage_entity;

            take_damage.notify(event);
            remove_hitpoints(damage, take_damage_entity);
            Audio::instance().play(SFXType::HIT);
            damage_given = true;
        });

        if (damage_given)
        {
            give_damage.reset_cooldown();
        }
    };

    registry.view<GiveJumpOnTopDamageComponent, PhysicsComponent, PositionComponent>().each(give_jump_on_top_damage);
}

void DamageSystem::update_spikes_damage()
{
    const float treshold_speed = 0.1f;

    auto& registry = EntityRegistry::instance().get_registry();

    auto spikes = registry.view<GiveSpikesDamageComponent, PhysicsComponent, PositionComponent>();
    auto bodies = registry.view<TakeSpikesDamageComponent, PhysicsComponent, PositionComponent>();

    auto give_spike_damage = [&](entt::entity spike_entity,
                                 GiveSpikesDamageComponent& spike_damage,
                                 PhysicsComponent& spike_physics,
                                 PositionComponent& spike_position)
    {
        bodies.each([&](entt::entity body_entity,
                             TakeSpikesDamageComponent& body_damage,
                             PhysicsComponent& body_physics,
                             PositionComponent& body_position)
        {
            if (body_physics.get_y_velocity() < treshold_speed)
            {
                return;
            }

            if (spike_physics.is_collision(body_physics, body_position, spike_position))
            {
                auto& hitpoints = registry.get<HitpointComponent>(body_entity);
                remove_hitpoints(hitpoints.get_hitpoints(), body_entity);

                // Assuming only spikes can deal spikes-damage:
                auto& quad = registry.get<QuadComponent>(spike_entity);
                quad.frame_changed(NPCSpritesheetFrames::SPIKES_BLOOD);
            }
        });
    };

    spikes.each(give_spike_damage);
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

                    remove_hitpoints(take_damage_hitpoints.get_hitpoints(), body_entity);
                    take_damage_explosion.notify({});
                }
            }
        });
    };

    bodies_giving_damage.each(give_explosion_damage);
}

void DamageSystem::update_npc_touch_damage(std::uint32_t delta_time_ms)
{
    // TODO: Problem - if damage upon jumping on top is inflicted, then there's still collision
    //       and damage upon touching NPC is given (shouldn't be!).
    //
    //       Not sure if this is a problem though; this counts only for NPC's that have more than 1 hitpoints, i.e caveman,
    //       and since caveman will have his GiveNpcDamageComponent removed upon being stunned, this effect won't take place.
    //
    //       Either way, remove this comment once stunnable caveman is implemented.

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
