#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

using FallDamage_t = int;
class TakeFallDamageComponent : public Subject<FallDamage_t>
{
public:
    TakeFallDamageComponent() = default;
    explicit TakeFallDamageComponent(FallDamage_t falling_damage)
        : _falling_damage(falling_damage)
    {}
    TakeFallDamageComponent(FallDamage_t falling_damage, float critical_spped)
        : _falling_damage(falling_damage)
        , _critical_speed(critical_spped)
    {}

    bool update(const PhysicsComponent& physics)
    {
        const bool damage_taken =  _last_tick_y_velocity > _critical_speed && physics.get_y_velocity() == 0.0f;
        _last_tick_y_velocity = physics.get_y_velocity();
        return damage_taken;
    }

    int get_falling_damage() const
    {
        return _falling_damage;
    }

private:
    FallDamage_t _falling_damage = 0;
    float _last_tick_y_velocity = 0.0f;
    float _critical_speed = 0.2f;
};
