//
// Created by xdbeef on 10.03.18.
//

#include "MapTile.hpp"

/**
 * Set tile properties, like:
 * -> byte 
 * -> can it be collided with main dude and AI's
 * -> can it be destroyed (by a bomb for example)
 * based on the given MapTileType.
 */
void MapTile::match_tile(MapTileType type) {
    // minus one since tile type 'NOTHING' is at the first place
    mapTileType = type;

    switch (type) {
        case LADDER:
            collidable = false;
            destroyable = false;
            break;
        case LADDER_DECK:
            collidable = false;
            destroyable = false;
            break;
        case ENTRANCE:
            collidable = false;
            destroyable = false;
            break;
        case EXIT:
            collidable = false;
            destroyable = false;
            break;
        case CONSOLE_LEFT_BAR_TOP_ROUNDED:
            collidable = false;
            destroyable = false;
            break;
        case CONSOLE_RIGHT_BAR_TOP_ROUNDED:
            collidable = false;
            destroyable = false;
            break;
        case CONSOLE_LEFT_BAR_BOT_ROUNDED:
            collidable = false;
            destroyable = false;
            break;
        case CONSOLE_RIGHT_BAR_BOT_ROUNDED:
            collidable = false;
            destroyable = false;
            break;
        case CONSOLE_TOP_BAR:
            collidable = false;
            destroyable = false;
            break;
        case CONSOLE_BOTTOM_BAR:
            collidable = false;
            destroyable = false;
            break;
        case CONSOLE_LEFT_BAR:
            collidable = false;
            destroyable = false;
            break;
        case CONSOLE_RIGHT_BAR:
            collidable = false;
            destroyable = false;
            break;
        case CONSOLE_BLACK_BACKGROUND:
            collidable = false;
            destroyable = false;
            break;
        case CAVE_SMOOTH:
            collidable = true;
            destroyable = true;
            break;
        case SCORES_STAR_DOOR:
            collidable = false;
            destroyable = false;
            break;
        case SCORES_SUN_DOOR:
            collidable = false;
            destroyable = false;
            break;
        case SCORES_MOON_DOOR:
            collidable = false;
            destroyable = false;
            break;
        case SCORES_CHANGING_DOOR:
            collidable = false;
            destroyable = false;
            break;
        case SHOP_SIGN_RARE:
            collidable = true;
            destroyable = true;
            break;
        case SHOP_SIGN_WEAPON:
            collidable = true;
            destroyable = true;
            break;
        case SHOP_SIGN_BOMBS:
            collidable = true;
            destroyable = true;
            break;
        case SHOP_SIGN_CLOTHING:
            collidable = true;
            destroyable = true;
            break;
        case SHOP_SIGN_CRAPS:
            collidable = true;
            destroyable = true;
            break;
        case SHOP_SIGN_GENERAL:
            collidable = true;
            destroyable = true;
            break;
        case SHOP_SIGN_KISSING:
            collidable = true;
            destroyable = true;
            break;
        case NA:
            collidable = false;
            destroyable = false;
            break;
        case SHOP_MUGSHOT_1:
            collidable = false;
            destroyable = false;
            break;
        case SHOP_MUGSHOT_2:
            collidable = false;
            destroyable = false;
            break;
        case SHOP_MUGSHOT_3:
            collidable = false;
            destroyable = false;
            break;
        case SHOP_MUGSHOT_4:
            collidable = false;
            destroyable = false;
            break;
        default:
            // most of the tiles are destroyable and collidable,
            // if happens to be otherwise, set it in a specific switch-case
            destroyable = true;
            collidable = true;
            break;
    }
}
