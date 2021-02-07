#pragma once

#include <cstdint>

enum class NPCType : std::uint16_t
{
    NOTHING = 0,
    ANY = 1,
    ARROW_TRAP_LEFT = 2,
    ARROW_TRAP_RIGHT = 3,
    SPIDER = 4
};
