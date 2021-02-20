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

private:
    JumpOnTopDamage_t _damage_given = 0;
};
