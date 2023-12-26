#include "other/ItemType.hpp"

const char *to_string(ItemType item_type) {
#define TO_STRING(x) case ItemType::x: return #x;
    switch (item_type) {
        TO_STRING(ARROW);
        TO_STRING(BOMB);
        TO_STRING(CAPE);
        TO_STRING(CHEST);
        TO_STRING(CRATE);
        TO_STRING(JAR);
        TO_STRING(JETPACK);
        TO_STRING(PISTOL);
        TO_STRING(ROCK);
        TO_STRING(ROPE);
        TO_STRING(SHOTGUN);
        TO_STRING(SKULL);
        TO_STRING(WHIP);
        TO_STRING(BOMB_SPAWNER);
        TO_STRING(ROPE_SPAWNER);
        TO_STRING(WALLET);
        TO_STRING(SPIKE_SHOES);
        TO_STRING(SPRING_SHOES);
        TO_STRING(MITT);
        TO_STRING(GLOVE);
        TO_STRING(COMPASS);
        TO_STRING(BODY);
        TO_STRING(BOMB_BAG);
        TO_STRING(ROPE_PILE);
        TO_STRING(GOLDEN_IDOL);
        TO_STRING(FLARE);
        TO_STRING(_SIZE);
    }
    assert(false);
    return "Failed to match passed ItemType";
}
