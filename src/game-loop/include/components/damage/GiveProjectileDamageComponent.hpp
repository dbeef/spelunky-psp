#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

using MutualDamage_t = int;
using ProjectileDamage_t = int;
class GiveProjectileDamageComponent : public Subject<MutualDamage_t>
{
public:

    explicit GiveProjectileDamageComponent(ProjectileDamage_t damage_given, entt::entity last_throw_source = entt::null)
        : _damage_given(damage_given)
        , _last_throw_source(last_throw_source)
    {}

    void set_mutual(bool mutual)
    {
        _mutual = mutual;
    }

    bool is_mutual() const
    {
        return _mutual;
    }

    ProjectileDamage_t get_damage() const
    {
        return _damage_given;
    }

    bool is_last_throw_source(entt::entity last_throw_source) const
    {
        return last_throw_source == _last_throw_source;
    }

    void set_last_throw_source(entt::entity last_throw_source)
    {
        _last_throw_source = last_throw_source;
    }

private:
    ProjectileDamage_t _damage_given = 0;
    entt::entity _last_throw_source;
    bool _mutual = false;
};
