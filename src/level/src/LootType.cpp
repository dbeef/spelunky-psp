#include "LootType.hpp"
#include <cassert>

const char *to_string(LootType loot_type) {
#define TO_STRING(x) case LootType::x: return #x;
    switch (loot_type) {
        TO_STRING(NOTHING);
        TO_STRING(ANY);
        TO_STRING(SHOP_ITEM);
        TO_STRING(GOLDEN_IDOL);
        TO_STRING(SINGLE_GOLD_BAR);
        TO_STRING(BIG_GEM);
        TO_STRING(CHEST);
        TO_STRING(JAR);
        TO_STRING(ROCK);
        TO_STRING(CRATE);
        TO_STRING(_SIZE);
    }
    assert(false);
    return "Failed to match passed LootType";
}
