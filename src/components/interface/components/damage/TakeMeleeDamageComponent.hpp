#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

using MeleeDamage_t = int;

struct TakenMeleeDamageEvent
{
    MeleeDamage_t amount;
    entt::entity source;
};

class TakeMeleeDamageComponent : public Subject<TakenMeleeDamageEvent>
{
};
