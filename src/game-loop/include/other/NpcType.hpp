#pragma once

#include <cassert>
#include <cstdint>

using NpcType_t = std::uint16_t;

enum class NpcType : NpcType_t
{
    NONE = 0,
    SNAKE,
    BAT,
    CAVEMAN,
    SPIDER,
    SKELETON,
    SHOPKEEPER,
    DAMSEL,
    BLUE_FROG,
    RED_FROG,
    _SIZE
};

const char* to_string(NpcType npc_type);