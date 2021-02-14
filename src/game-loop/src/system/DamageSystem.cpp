#include "system/DamageSystem.hpp"
#include "components/damage/TakeFallDamageComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/damage/HitpointComponent.hpp"
#include "EntityRegistry.hpp"
#include "audio/Audio.hpp"

void DamageSystem::update(std::uint32_t delta_time_ms)
{
    update_falling_damage();
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
