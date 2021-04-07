#pragma once

#include "patterns/Subject.hpp"

enum class ClimbingEventType
{
    STARTED_CLIMBING_LADDER,
    STARTED_CLIMBING_ROPE,
};

struct ClimbingEvent
{
    ClimbingEventType event_type;
};

struct ClimbingComponent : Subject<ClimbingEvent>
{
};
