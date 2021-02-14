#include "system/DamageSystem.hpp"
#include "components/damage/TakeFallDamageComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "components/damage/TakeProjectileDamageComponent.hpp"
#include "components/damage/GiveProjectileDamageComponent.hpp"
#include "EntityRegistry.hpp"
#include "audio/Audio.hpp"

void DamageSystem::update(std::uint32_t delta_time_ms)
{
    update_falling_damage();
    update_projectile_damage();
}

void DamageSystem::update_projectile_damage()
{
    auto &registry = EntityRegistry::instance().get_registry();
    auto bodies = registry.view<TakeProjectileDamageComponent, HitpointComponent, PhysicsComponent, PositionComponent>();
    auto projectiles = registry.view<GiveProjectileDamageComponent, PhysicsComponent, PositionComponent>();

    auto give_projectile_damage = [&bodies](GiveProjectileDamageComponent& give_damage,
                                            PhysicsComponent& projectile_physics,
                                            PositionComponent& projectile_position)
    {
        if (projectile_physics.get_x_velocity() == 0.0f && projectile_physics.get_y_velocity() == 0.0f)
        {
            return;
        }

        bodies.each([&projectile_physics, &projectile_position, &give_damage](
                        TakeProjectileDamageComponent& take_damage,
                        HitpointComponent& hitpoints,
                        PhysicsComponent& body_physics,
                        PositionComponent& body_position)
        {
            if (!body_physics.is_collision(projectile_physics, projectile_position, body_position))
            {
                return;
            }

            const ProjectileDamage_t damage = give_damage.get_damage();

            take_damage.notify(damage);
            hitpoints.remove_hitpoints(damage);

            if (hitpoints.get_hitpoints() <= 0)
            {
                hitpoints.notify({});
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
