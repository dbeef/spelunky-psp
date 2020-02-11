//
// Created by dbeef on 2/5/20.
//

#include <RenderTile.hpp>
#include <logger/log.h>
#include <glad/glad.h>

namespace
{
    const char* to_string(MapTileType);
    void parse_dimensions(RenderTile& tile, tao::json::value &sprite);
    void parse_indices(RenderTile& tile, tao::json::value &sprite);
    void parse_positions(RenderTile& tile, tao::json::value &sprite);
    void parse_uvs(RenderTile& tile, tao::json::value &sprite);
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

            parse_dimensions(tile, sprite);
            parse_indices(tile, sprite);
            parse_positions(tile, sprite);
            parse_uvs(tile, sprite);

            log_info("Parsed tile: %s (%s)", filename.c_str(), to_string(tile.type));
            return tile;
        }
    }

    log_error("Failed to parse tile: %i (%s)", tile_index, to_string(tile.type));
    assert(false);
    return tile;
}

void RenderTile::normalize(std::uint16_t spritesheet_width, std::uint16_t spritesheet_height)
{
    for (std::size_t index = 0; index < 4; index++)
    {
        positions_normalized[index][0] = static_cast<float>(positions[index][0]) / width;
        positions_normalized[index][1] = static_cast<float>(positions[index][1]) / height;
    }

    uv[0][0]+=1;
    uv[0][1]+=1;

    if (uv[1][0] - 1 >= 0) uv[1][0]-=1;
    uv[1][1]+=1;

    if (uv[2][0] - 1 >= 0) uv[2][0]-=1;
    if (uv[2][1] - 1 >= 0) uv[2][1]-=1;

    uv[3][0]+=1;
    if (uv[3][1] - 1 >= 0) uv[3][1]-=1;

    for (std::size_t index = 0; index < 4; index++)
    {
        uv_normalized[index][0] = static_cast<float>(uv[index][0]) / spritesheet_width;
        uv_normalized[index][1] = static_cast<float>(uv[index][1]) / spritesheet_height;
    }
}

void RenderTile::push_uvs(std::vector<GLfloat> &out_uvs)
{
    for(std::size_t x = 0; x < 4; x++)
    {
        out_uvs.push_back(uv_normalized[x][0]);
        out_uvs.push_back(uv_normalized[x][1]);
    }
}

void RenderTile::push_positions(std::vector<GLfloat> &out_positions, int x_offset, int y_offset)
{
    for(std::size_t x = 0; x < 4; x++)
    {
        out_positions.push_back(positions_normalized[x][0] + x_offset);
        out_positions.push_back(positions_normalized[x][1] + y_offset);
    }
}

void RenderTile::push_indices(std::vector<GLuint> &out_indices, std::size_t offset)
{
    for(std::size_t x = 0; x < 6; x++)
    {
        out_indices.push_back(indices[x] + (offset * 4));
    }
}

namespace
{
#define caseToString(X) case X: return #X;

    const char *to_string(MapTileType type)
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

    void parse_dimensions(RenderTile &tile, tao::json::value &sprite)
    {
        auto dimensions = sprite["size"].get_array();
        assert(dimensions.size() == 2);

        tile.width = dimensions[0].get_unsigned();
        tile.height = dimensions[1].get_unsigned();
    }

    void parse_indices(RenderTile &tile, tao::json::value &sprite)
    {
        auto indices = sprite["mesh"]["indices"].get_array();
        assert(indices.size() == 6);

        for (std::size_t index = 0; index < 6; index++)
        {
            tile.indices[index] = indices[index].get_unsigned();
        }
    }

    void parse_positions(RenderTile &tile, tao::json::value &sprite)
    {
        auto positions = sprite["mesh"]["positions"].get_array();
        assert(positions.size() == 4);

        for (std::size_t index = 0; index < 4; index++)
        {
            auto xy = positions[index].get_array();
            assert(xy.size() == 2);

            tile.positions[index][0] = xy[0].get_unsigned();
            tile.positions[index][1] = xy[1].get_unsigned();
        }
    }

    void parse_uvs(RenderTile &tile, tao::json::value &sprite)
    {
        auto uvs = sprite["mesh"]["uvs"].get_array();
        assert(uvs.size() == 4);

        for (std::size_t index = 0; index < 4; index++)
        {
            auto xy = uvs[index].get_array();
            assert(xy.size() == 2);

            tile.uv[index][0] = xy[0].get_unsigned();
            tile.uv[index][1] = xy[1].get_unsigned();
        }
    }

#undef caseToString
}
