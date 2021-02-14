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

private:
    MeleeDamage_t _damage_given = 0;
};
