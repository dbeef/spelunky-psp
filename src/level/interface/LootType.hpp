#pragma once

#include <cstdint>

enum class LootType : std::uint16_t
{
    NOTHING = 0,
    GOLD_BAR,
    TRIPLE_GOLD_BAR,
};
