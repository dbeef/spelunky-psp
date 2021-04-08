#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

using NpcDamage_t = int;
struct GiveNpcTouchDamageComponent : public Subject<NpcDamage_t>
{
    static const int TOP_COOLDOWN = 500;
    int cooldown = TOP_COOLDOWN;
    // TODO: Possibly amount of damage, if not always "1"
};
