#pragma once

#include "patterns/Subject.hpp"

struct SpikesDamageEvent
{};

struct TakeSpikesDamageComponent : public Subject<SpikesDamageEvent>
{
    // https://github.com/skypjack/entt/issues/330
    char dummy_member_for_entt;
};
