#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

using MeleeDamage_t = int;
class TakeMeleeDamageComponent : public Subject<MeleeDamage_t>
{
};
