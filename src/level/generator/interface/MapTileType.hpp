//
// Created by xdbeef on 27.04.18.
//

#pragma once

#include <cstdint>

enum class MapTileType : std::uint32_t {
    NOTHING = 0,                    // 0 NON_COLLIDABLE
    CAVE_ROCK,                      // 1
    CAVE_REGULAR,                   // 2
    STONE_BLOCK,                    // 3
    CAVE_DOWN_ORIENTED,             // 4
    CAVE_SOME_GOLD,                 // 5
    CAVE_MUCH_GOLD,                 // 6
    CAVE_UP_ORIENTED,               // 7
    CAVE_UP_DOWN_ORIENTED,          // 8
    LADDER,                         // 9  NON_COLLIDABLE
    LADDER_DECK,                    // 10 NON_COLLIDABLE
    ARROW_TRAP_LEFT,                // 11
    ARROW_TRAP_RIGHT,               // 12
    ENTRANCE,                       // 13 NON_COLLIDABLE
    EXIT,                           // 14 NON_COLLIDABLE
    CONSOLE_LEFT_BAR_TOP_ROUNDED,   // 15
    CONSOLE_RIGHT_BAR_TOP_ROUNDED,  // 16
    CONSOLE_LEFT_BAR_BOT_ROUNDED,   // 17
    CONSOLE_RIGHT_BAR_BOT_ROUNDED,  // 18
    CONSOLE_TOP_BAR,                // 19
    CONSOLE_BOTTOM_BAR,             // 20
    CONSOLE_LEFT_BAR,               // 21
    CONSOLE_RIGHT_BAR,              // 22
    CONSOLE_BLACK_BACKGROUND,       // 23
    CAVE_SMOOTH,                    // 24
    SCORES_STAR_DOOR,               // 25
    SCORES_SUN_DOOR,                // 26
    SCORES_MOON_DOOR,               // 27
    SCORES_CHANGING_DOOR,           // 28
    SHOP_SIGN_RARE,                 // 29
    SHOP_SIGN_WEAPON,               // 30
    SHOP_SIGN_BOMBS,                // 31
    SHOP_SIGN_CLOTHING,             // 32
    SHOP_SIGN_CRAPS,                // 33
    SHOP_SIGN_GENERAL,              // 34
    SHOP_SIGN_KISSING,              // 35
    NA,                             // 36
    SHOP_MUGSHOT_1,                 // 37
    SHOP_MUGSHOT_2,                 // 38
    SHOP_MUGSHOT_3,                 // 39
    SHOP_MUGSHOT_4,                 // 40
    ALTAR_LEFT,                     // 41
    ALTAR_RIGHT,                    // 42
    CAVE_BG_1,                      // 43
    CAVE_BG_2,                      // 44
    CAVE_BG_3,                      // 45
    CAVE_BG_4,                      // 46
    _SIZE                           // 47 Total elements
};
