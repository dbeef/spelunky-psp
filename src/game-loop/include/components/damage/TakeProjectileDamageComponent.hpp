#pragma once

#include "patterns/Subject.hpp"
#include "components/generic/PhysicsComponent.hpp"

using ProjectileDamage_t = int;
class TakeProjectileDamageComponent : public Subject<ProjectileDamage_t>
{
};
