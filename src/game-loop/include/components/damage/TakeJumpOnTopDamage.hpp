#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

using JumpOnTopDamage_t = int;
class TakeJumpOnTopDamageComponent : public Subject<JumpOnTopDamage_t>
{
};
