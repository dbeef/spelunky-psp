//
// Created by xdbeef on 10.03.18.
//

#include "MapTile.hpp"

void MapTile::match_tile(MapTileType type)
{
    map_tile_type = type;

    switch (type)
    {
        default:
        {
            // Most of the tiles are destroyable and collidable,
            // if it happens to be otherwise, set it in a specific switch-case.
            destroyable = true;
            collidable = true;
            break;
        }
        case MapTileType::LADDER:
        {
            collidable = false;
            destroyable = false;
            climbable = true;
            break;
        }
        case MapTileType::LADDER_DECK:
        {
            collidable = false;
            destroyable = false;
            climbable = true;
            break;
        }
        case MapTileType::ENTRANCE:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::EXIT:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::CONSOLE_LEFT_BAR_TOP_ROUNDED:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::CONSOLE_RIGHT_BAR_TOP_ROUNDED:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::CONSOLE_LEFT_BAR_BOT_ROUNDED:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::CONSOLE_RIGHT_BAR_BOT_ROUNDED:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::CONSOLE_TOP_BAR:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::CONSOLE_BOTTOM_BAR:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::CONSOLE_LEFT_BAR:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::CONSOLE_RIGHT_BAR:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::CONSOLE_BLACK_BACKGROUND:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::CAVE_SMOOTH:
        {
            collidable = true;
            destroyable = true;
            climbable = false;
            break;
        }
        case MapTileType::CAVE_REGULAR:
        {
            collidable = true;
            destroyable = true;
            climbable = false;
            break;
        }
        case MapTileType::SCORES_STAR_DOOR:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::SCORES_SUN_DOOR:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::SCORES_MOON_DOOR:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::SCORES_CHANGING_DOOR:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::SHOP_SIGN_RARE:
        {
            collidable = true;
            destroyable = true;
            climbable = false;
            break;
        }
        case MapTileType::SHOP_SIGN_WEAPON:
        {
            collidable = true;
            destroyable = true;
            climbable = false;
            break;
        }
        case MapTileType::SHOP_SIGN_BOMBS:
        {
            collidable = true;
            destroyable = true;
            climbable = false;
            break;
        }
        case MapTileType::SHOP_SIGN_CLOTHING:
        {
            collidable = true;
            destroyable = true;
            climbable = false;
            break;
        }
        case MapTileType::SHOP_SIGN_CRAPS:
        {
            collidable = true;
            destroyable = true;
            climbable = false;
            break;
        }
        case MapTileType::SHOP_SIGN_GENERAL:
        {
            collidable = true;
            destroyable = true;
            climbable = false;
            break;
        }
        case MapTileType::SHOP_SIGN_KISSING:
        {
            collidable = true;
            destroyable = true;
            climbable = false;
            break;
        }
        case MapTileType::NA:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::SHOP_MUGSHOT_1:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::SHOP_MUGSHOT_2:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::SHOP_MUGSHOT_3:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::SHOP_MUGSHOT_4:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
        case MapTileType::CAVE_BG_1:
        case MapTileType::CAVE_BG_2:
        case MapTileType::CAVE_BG_3:
        case MapTileType::CAVE_BG_4:
        {
            collidable = false;
            destroyable = false;
            climbable = false;
            break;
        }
    }
}
