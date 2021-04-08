#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

using NpcDamage_t = int;
struct GiveNpcTouchDamageComponent : public Subject<NpcDamage_t>
{
    static const int TOP_COOLDOWN_MS = 1000;
    int cooldown_ms = TOP_COOLDOWN_MS;
};
