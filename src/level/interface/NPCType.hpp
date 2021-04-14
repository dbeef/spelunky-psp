#pragma once

#include <cstdint>

// FIXME: This seems to be a duplicate of "NpcType.hpp" from the GameLoop
enum class NPCType : std::uint16_t
{
    NOTHING = 0,
    ANY = 1,
    ARROW_TRAP_LEFT = 2,
    ARROW_TRAP_RIGHT = 3,
    SPIDER = 4,
    SHOPKEEPER = 5
};
