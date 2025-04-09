#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include <entt/entt.hpp>

using JumpOnTopDamage_t = int;

struct TakenJumpOnTopDamageEvent
{
    JumpOnTopDamage_t amount;
    entt::entity source;
};

class TakeJumpOnTopDamageComponent : public Subject<TakenJumpOnTopDamageEvent>
{
};
