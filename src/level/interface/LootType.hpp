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
    SINGLE_GOLD_BAR,
    BIG_GEM,
    CHEST,
    JAR,
    ROCK,
    CRATE,
    _SIZE
};

const char* to_string(LootType);