#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

#include <cmath>

using TileCollisionDamage_t = int;
class TakeTileCollisionDamageComponent : public Subject<TileCollisionDamage_t>
{
public:
    TakeTileCollisionDamageComponent() = default;
    explicit TakeTileCollisionDamageComponent(TileCollisionDamage_t tile_collision_damage,
                                             float critical_velocity_x,
                                             float critical_velocity_y)
        : _tile_collision_damage(tile_collision_damage)
        , _critical_velocity { critical_velocity_x, critical_velocity_y }
    {}

    bool update(const PhysicsComponent& physics)
    {
        const bool damage_taken =  (physics.get_y_velocity() == 0.0f && std::fabs(_last_tick_velocity.y) > _critical_velocity.y) ||
                                   (physics.get_x_velocity() == 0.0f && std::fabs(_last_tick_velocity.x) > _critical_velocity.x);

        _last_tick_velocity.x = physics.get_x_velocity();
        _last_tick_velocity.y = physics.get_y_velocity();

        return damage_taken;
    }

    TileCollisionDamage_t get_tile_collision_damage() const
    {
        return _tile_collision_damage;
    }

private:
    TileCollisionDamage_t _tile_collision_damage = 0;
    
    struct
    {
        float x = 0.0f;
        float y = 0.0f;
    } _last_tick_velocity;
    
    struct
    {
        float x = 0.0f;
        float y = 0.0f;
    } _critical_velocity;
};
