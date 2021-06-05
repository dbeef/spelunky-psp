#pragma once

#include "MapTileType.hpp"
#include <cassert>

const char *toString(MapTileType tile) {
#define STR(V) case MapTileType::V: return #V;
    switch (tile) {
        STR(NOTHING);
        STR(CAVE_ROCK);
        STR(CAVE_REGULAR);
        STR(STONE_BLOCK);
        STR(CAVE_DOWN_ORIENTED);
        STR(CAVE_SOME_GOLD);
        STR(CAVE_MUCH_GOLD);
        STR(CAVE_UP_ORIENTED);
        STR(CAVE_UP_DOWN_ORIENTED);
        STR(LADDER);
        STR(LADDER_DECK);
        STR(ARROW_TRAP_LEFT);
        STR(ARROW_TRAP_RIGHT);
        STR(ENTRANCE);
        STR(EXIT);
        STR(CONSOLE_LEFT_BAR_TOP_ROUNDED);
        STR(CONSOLE_RIGHT_BAR_TOP_ROUNDED);
        STR(CONSOLE_LEFT_BAR_BOT_ROUNDED);
        STR(CONSOLE_RIGHT_BAR_BOT_ROUNDED);
        STR(CONSOLE_TOP_BAR);
        STR(CONSOLE_BOTTOM_BAR);
        STR(CONSOLE_LEFT_BAR);
        STR(CONSOLE_RIGHT_BAR);
        STR(CONSOLE_BLACK_BACKGROUND);
        STR(CAVE_SMOOTH);
        STR(SCORES_STAR_DOOR);
        STR(SCORES_SUN_DOOR);
        STR(SCORES_MOON_DOOR);
        STR(SCORES_CHANGING_DOOR);
        STR(SHOP_SIGN_RARE);
        STR(SHOP_SIGN_WEAPON);
        STR(SHOP_SIGN_BOMBS);
        STR(SHOP_SIGN_CLOTHING);
        STR(SHOP_SIGN_CRAPS);
        STR(SHOP_SIGN_GENERAL);
        STR(SHOP_SIGN_KISSING);
        STR(NA);
        STR(SHOP_MUGSHOT_1);
        STR(SHOP_MUGSHOT_2);
        STR(SHOP_MUGSHOT_3);
        STR(SHOP_MUGSHOT_4);
        STR(ALTAR_LEFT);
        STR(ALTAR_RIGHT);
        STR(CAVE_BG_1);
        STR(CAVE_BG_2);
        STR(CAVE_BG_3);
        STR(CAVE_BG_4);
        STR(_SIZE);
        default: assert(false); return "UNDEFINED!";
    }
}
