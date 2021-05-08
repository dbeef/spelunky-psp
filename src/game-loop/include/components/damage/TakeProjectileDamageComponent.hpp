#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

using ProjectileDamage_t = int;

struct TakenProjectileDamageEvent
{
    ProjectileDamage_t amount;
    entt::entity source;
};

class TakeProjectileDamageComponent : public Subject<TakenProjectileDamageEvent>
{
};
