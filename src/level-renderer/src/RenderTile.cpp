//
// Created by dbeef on 2/5/20.
//

#include "RenderTile.hpp"
#include "logger/log.h"
#include "cJSON.h"
#include "glad/glad.h"

#include <cstdlib>
#include <sstream>

namespace
{
    const char* to_string(MapTileType);
    void parse_dimensions(RenderTile& tile, cJSON* sprite);
    void parse_indices(RenderTile& tile, cJSON* sprite);
    void parse_positions(RenderTile& tile, cJSON* sprite);
    void parse_uvs(RenderTile& tile, cJSON* sprite);
}

RenderTile RenderTile::fromJson(MapTileType type, void* document_root)
{
    RenderTile tile{};
    tile.type = type;

    auto tile_index = static_cast<std::uint32_t>(type);

    cJSON* sprites_array = cJSON_GetObjectItemCaseSensitive(reinterpret_cast<cJSON*>(document_root), "sprites");
    assert (sprites_array && cJSON_IsArray(sprites_array));

    std::stringstream s;
    s << tile_index;
    s << ".png";
    const std::string filename = s.str();

    for (std::uint16_t index = 0; index < static_cast<std::uint16_t>(MapTileType::_SIZE); index++)
    {
        cJSON* sprite_document = cJSON_GetArrayItem(sprites_array, index);
        assert(sprite_document);
        cJSON* sprite_filename_document = cJSON_GetObjectItemCaseSensitive(sprite_document, "name");
        assert(sprite_filename_document && cJSON_IsString(sprite_filename_document));

        const std::string image_filename = cJSON_GetStringValue(sprite_filename_document);

        if (filename == image_filename)
        {
            parse_dimensions(tile, sprite_document);
            parse_indices(tile, sprite_document);
            parse_positions(tile, sprite_document);
            parse_uvs(tile, sprite_document);

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

void RenderTile::push_positions(std::vector<int16_t > &out_positions, int x_offset, int y_offset)
{
    for(std::size_t x = 0; x < 4; x++)
    {
        out_positions.push_back(positions_normalized[x][0] + x_offset);
        out_positions.push_back(positions_normalized[x][1] + y_offset);
    }
}

void RenderTile::push_indices(std::vector<std::int16_t> &out_indices, std::size_t offset)
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

    void parse_dimensions(RenderTile &tile, cJSON* sprite)
    {
        cJSON* dimensions_array = cJSON_GetObjectItemCaseSensitive(sprite, "size");
        assert(dimensions_array && cJSON_IsArray(dimensions_array) && cJSON_GetArraySize(dimensions_array) == 2);

        tile.width = cJSON_GetArrayItem(dimensions_array, 0)->valueint;
        tile.height = cJSON_GetArrayItem(dimensions_array, 1)->valueint;
    }

    void parse_indices(RenderTile &tile, cJSON* sprite)
    {
        cJSON* mesh_document = cJSON_GetObjectItemCaseSensitive(sprite, "mesh");
        assert(mesh_document);
        cJSON* indices_array = cJSON_GetObjectItemCaseSensitive(mesh_document, "indices");
        assert(indices_array && cJSON_IsArray(indices_array) && cJSON_GetArraySize(indices_array) == 6);

        for (std::size_t index = 0; index < 6; index++)
        {
            tile.indices[index] = cJSON_GetArrayItem(indices_array, index)->valueint;
        }
    }

    void parse_positions(RenderTile &tile, cJSON* sprite)
    {
        cJSON* mesh_document = cJSON_GetObjectItemCaseSensitive(sprite, "mesh");
        assert(mesh_document);
        cJSON* positions_array = cJSON_GetObjectItemCaseSensitive(mesh_document, "positions");
        assert(positions_array && cJSON_IsArray(positions_array) && cJSON_GetArraySize(positions_array) == 4);

        for (std::size_t index = 0; index < 4; index++)
        {
            auto xy = cJSON_GetArrayItem(positions_array, index);
            assert(xy && cJSON_IsArray(xy) && cJSON_GetArraySize(xy) == 2);

            tile.positions[index][0] = cJSON_GetArrayItem(xy, 0)->valueint;
            tile.positions[index][1] = cJSON_GetArrayItem(xy, 1)->valueint;
        }
    }

    void parse_uvs(RenderTile &tile, cJSON* sprite)
    {
        cJSON* mesh_document = cJSON_GetObjectItemCaseSensitive(sprite, "mesh");
        assert(mesh_document);
        cJSON* uvs_array = cJSON_GetObjectItemCaseSensitive(mesh_document, "uvs");
        assert(uvs_array && cJSON_IsArray(uvs_array) && cJSON_GetArraySize(uvs_array) == 4);

        for (std::size_t index = 0; index < 4; index++)
        {
            auto xy = cJSON_GetArrayItem(uvs_array, index);
            assert(xy && cJSON_IsArray(xy) && cJSON_GetArraySize(xy) == 2);

            tile.uv[index][0] = cJSON_GetArrayItem(xy, 0)->valueint;
            tile.uv[index][1] = cJSON_GetArrayItem(xy, 1)->valueint;
        }
    }

#undef caseToString
}
