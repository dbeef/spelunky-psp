#pragma once

#include "patterns/Subject.hpp"

struct DieEvent {};
using Hitpoint_t = int;

class HitpointComponent : public Subject<DieEvent>
{
public:
    explicit HitpointComponent(Hitpoint_t hitpoints) : _hitpoints(hitpoints) {}
    HitpointComponent() = default;

    void remove_hitpoints(Hitpoint_t hitpoints)
    {
        _hitpoints -= hitpoints;
    }

    Hitpoint_t get_hitpoints() const
    {
        return _hitpoints;
    }

private:
    Hitpoint_t _hitpoints = 0;
};
