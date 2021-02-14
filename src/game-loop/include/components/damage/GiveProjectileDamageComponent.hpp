#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

using ProjectileDamage_t = int;
class GiveProjectileDamageComponent
{
public:
    explicit GiveProjectileDamageComponent(ProjectileDamage_t damage_given)
        : _damage_given(damage_given)
    {}

    void set_mutual(bool mutual)
    {
        _mutual = mutual;
    }

    ProjectileDamage_t get_damage() const
    {
        return _damage_given;
    }

private:
    ProjectileDamage_t _damage_given = 0;
    bool _mutual = false;
};
