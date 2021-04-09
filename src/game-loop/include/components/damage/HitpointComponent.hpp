#pragma once

#include "patterns/Subject.hpp"
#include "other/NpcType.hpp"

struct DeathEvent
{
    NpcType npc_type;
};

using Hitpoint_t = int;

class HitpointComponent : public Subject<DeathEvent>
{
public:
    explicit HitpointComponent(Hitpoint_t hitpoints, bool dispose_when_zero)
        : _hitpoints(hitpoints)
        , _dispose_when_zero(dispose_when_zero)
    {}

    HitpointComponent() = default;

    void remove_hitpoints(Hitpoint_t hitpoints)
    {
        _hitpoints -= hitpoints;
    }

    Hitpoint_t get_hitpoints() const
    {
        return _hitpoints;
    }

    bool is_disposed_when_zero() const { return _dispose_when_zero; }

private:
    Hitpoint_t _hitpoints = 0;
    bool _dispose_when_zero = false;
};
