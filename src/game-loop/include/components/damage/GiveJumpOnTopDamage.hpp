#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

using JumpOnTopDamage_t = int;
class GiveJumpOnTopDamageComponent
{
public:
    explicit GiveJumpOnTopDamageComponent(JumpOnTopDamage_t damage_given)
            : _damage_given(damage_given)
    {}

    JumpOnTopDamage_t get_damage() const
    {
        return _damage_given;
    }

    bool cooldown_reached() const { return _cooldown_ms >= TOP_COOLDOWN_MS; }
    void reset_cooldown() { _cooldown_ms = 0; }
    void update_cooldown(uint32_t delta_t_ms) { _cooldown_ms += delta_t_ms; }

private:
    JumpOnTopDamage_t _damage_given = 0;

    static const int TOP_COOLDOWN_MS = 250;
    int _cooldown_ms = TOP_COOLDOWN_MS;
};
