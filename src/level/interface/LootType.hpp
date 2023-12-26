#pragma once

#include <cstdint>

using LootType_t = std::uint16_t;
enum class LootType : LootType_t
{
    // FIXME: Does not represent all types of loot
    NOTHING = 0,
    ANY = 1,
    SHOP_ITEM = 2,
    GOLDEN_IDOL = 3,
    _SIZE = 4
};

const char* to_string(LootType);