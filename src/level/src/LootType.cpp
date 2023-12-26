#include "LootType.hpp"
#include <cassert>

const char *to_string(LootType loot_type) {
#define TO_STRING(x) case LootType::x: return #x;
    switch (loot_type) {
        TO_STRING(NOTHING);
        TO_STRING(ANY);
        TO_STRING(SHOP_ITEM);
        TO_STRING(GOLDEN_IDOL);
        TO_STRING(_SIZE);
    }
    assert(false);
    return "Failed to match passed LootType";
}
