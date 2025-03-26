#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

using NpcDamage_t = int;
class TakeNpcTouchDamageComponent : public Subject<NpcDamage_t>
{
};
