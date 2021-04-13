#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

using MeleeDamage_t = int;
class GiveMeleeDamageComponent
{
public:
    explicit GiveMeleeDamageComponent(MeleeDamage_t damage_given)
            : _damage_given(damage_given)
    {}

    MeleeDamage_t get_damage() const
    {
        return _damage_given;
    }

    bool cooldown_reached() const { return _cooldown_ms >= TOP_COOLDOWN_MS; }
    void reset_cooldown() { _cooldown_ms = 0; }
    void update_cooldown(uint32_t delta_t_ms) { _cooldown_ms += delta_t_ms; }

private:
    MeleeDamage_t _damage_given = 0;

    static const int TOP_COOLDOWN_MS = 1000;
    int _cooldown_ms = TOP_COOLDOWN_MS;
};
