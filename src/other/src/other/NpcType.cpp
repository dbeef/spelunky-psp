#include "other/NpcType.hpp"

const char *to_string(NpcType npc_type) {
#define TO_STRING(x) case NpcType::x: return #x;
    switch (npc_type) {
        TO_STRING(NONE);
        TO_STRING(SNAKE);
        TO_STRING(BAT);
        TO_STRING(CAVEMAN);
        TO_STRING(SPIDER);
        TO_STRING(SKELETON);
        TO_STRING(SHOPKEEPER);
        TO_STRING(DAMSEL);
        TO_STRING(BLUE_FROG);
        TO_STRING(RED_FROG);
    }
    assert(false);
    return "Failed to match passed NpcType";
}
