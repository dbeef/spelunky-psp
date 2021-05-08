#pragma once

#include <cstdint>

enum class LootType : std::uint16_t
{
    NOTHING = 0,
    ANY = 1,
    SHOP_ITEM = 2,
    GOLDEN_IDOL = 3
};
