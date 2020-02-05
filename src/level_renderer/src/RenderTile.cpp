//
// Created by dbeef on 2/5/20.
//

#include <RenderTile.hpp>
#include <logger/log.h>

namespace
{
    const char* toString(MapTileType);

}

RenderTile RenderTile::fromJson(MapTileType type, tao::json::value &document_root)
{
    RenderTile tile{};
    tile.type = type;

    auto tile_index = static_cast<std::uint32_t>(type);

    auto& sprites_array = document_root["sprites"].get_array();
    const std::string filename = std::to_string(tile_index) + ".png";

    for (std::uint16_t index = 0; index < static_cast<std::uint16_t>(MapTileType::_SIZE); index++)
    {
        const std::string image_filename = sprites_array[index]["name"].get_string();

        if (filename == image_filename)
        {
            auto sprite = sprites_array[index];
            tile.width = sprite["size"].get_array()[0].get_unsigned();
            tile.height = sprite["size"].get_array()[1].get_unsigned();

            log_info("Parsed tile: %s (%s)", filename.c_str(), toString(tile.type));
            return tile;
        }
    }

    log_error("Failed to parse tile: %i (%s)", tile_index, toString(tile.type));
    assert(false);
    return tile;
}

namespace
{
#define caseToString(X) case X: return #X;

    const char *toString(MapTileType type)
    {
        switch (type)
        {
            caseToString(MapTileType::NOTHING)
            caseToString(MapTileType::CAVE_ROCK)
            caseToString(MapTileType::CAVE_REGULAR)
            caseToString(MapTileType::STONE_BLOCK)
            caseToString(MapTileType::CAVE_DOWN_ORIENTED)
            caseToString(MapTileType::CAVE_SOME_GOLD)
            caseToString(MapTileType::CAVE_MUCH_GOLD)
            caseToString(MapTileType::CAVE_UP_ORIENTED)
            caseToString(MapTileType::CAVE_UP_DOWN_ORIENTED)
            caseToString(MapTileType::LADDER)
            caseToString(MapTileType::LADDER_DECK)
            caseToString(MapTileType::ARROW_TRAP_LEFT)
            caseToString(MapTileType::ARROW_TRAP_RIGHT)
            caseToString(MapTileType::ENTRANCE)
            caseToString(MapTileType::EXIT)
            caseToString(MapTileType::CONSOLE_LEFT_BAR_TOP_ROUNDED)
            caseToString(MapTileType::CONSOLE_RIGHT_BAR_TOP_ROUNDED)
            caseToString(MapTileType::CONSOLE_LEFT_BAR_BOT_ROUNDED)
            caseToString(MapTileType::CONSOLE_RIGHT_BAR_BOT_ROUNDED)
            caseToString(MapTileType::CONSOLE_TOP_BAR)
            caseToString(MapTileType::CONSOLE_BOTTOM_BAR)
            caseToString(MapTileType::CONSOLE_LEFT_BAR)
            caseToString(MapTileType::CONSOLE_RIGHT_BAR)
            caseToString(MapTileType::CONSOLE_BLACK_BACKGROUND)
            caseToString(MapTileType::CAVE_SMOOTH)
            caseToString(MapTileType::SCORES_STAR_DOOR)
            caseToString(MapTileType::SCORES_SUN_DOOR)
            caseToString(MapTileType::SCORES_MOON_DOOR)
            caseToString(MapTileType::SCORES_CHANGING_DOOR)
            caseToString(MapTileType::SHOP_SIGN_RARE)
            caseToString(MapTileType::SHOP_SIGN_WEAPON)
            caseToString(MapTileType::SHOP_SIGN_BOMBS)
            caseToString(MapTileType::SHOP_SIGN_CLOTHING)
            caseToString(MapTileType::SHOP_SIGN_CRAPS)
            caseToString(MapTileType::SHOP_SIGN_GENERAL)
            caseToString(MapTileType::SHOP_SIGN_KISSING)
            caseToString(MapTileType::NA)
            caseToString(MapTileType::SHOP_MUGSHOT_1)
            caseToString(MapTileType::SHOP_MUGSHOT_2)
            caseToString(MapTileType::SHOP_MUGSHOT_3)
            caseToString(MapTileType::SHOP_MUGSHOT_4)
            caseToString(MapTileType::ALTAR_LEFT)
            caseToString(MapTileType::ALTAR_RIGHT)
            caseToString(MapTileType::_SIZE)
            default:
                assert(false);
                return "Undefined MapTileType value";
        }
    }

#undef caseToString
}
