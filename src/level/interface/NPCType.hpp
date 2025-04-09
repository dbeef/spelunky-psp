#pragma once

#include <cstdint>

// FIXME: This seems to be a duplicate of "NpcType.hpp" from the GameLoop
enum class NPCType : std::uint16_t
{
    NOTHING = 0,
    ANY = 1,
    SNAKE,
    BAT,
    CAVEMAN,
    FAKE_SKELETON,
    SKELETON,
    SPIKES,
    SPIDER,
    DAMSEL,
    ARROW_TRAP_LEFT,
    ARROW_TRAP_RIGHT,
    SHOPKEEPER,
    _SIZE
};
