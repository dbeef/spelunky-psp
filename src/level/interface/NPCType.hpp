#pragma once

#include <cstdint>

enum class NPCType : std::uint16_t
{
    NOTHING = 0,
    ANY,
    ARROW_TRAP_LEFT,
    ARROW_TRAP_RIGHT
};
