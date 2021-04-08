#pragma once

#include "patterns/Subject.hpp"

struct ExplosionDamageTakenEvent
{
};

class TakeExplosionDamageComponent : public Subject<ExplosionDamageTakenEvent>
{
};
