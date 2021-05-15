#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include <vector>

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

    std::vector<entt::entity>& get_last_update_overlaping_bodies()
    {
        return _last_update_overlaping_bodies;
    }

private:
    std::vector<entt::entity> _last_update_overlaping_bodies;
    MeleeDamage_t _damage_given = 0;
};
