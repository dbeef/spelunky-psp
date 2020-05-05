//
// Created by dbeef on 2/5/20.
//

#include "TextureRegion.hpp"
#include "logger/log.h"
#include "cJSON.h"
#include "glad/glad.h"

#include <cstdlib>
#include <cassert>
#include <limits>
#include <sstream>

namespace
{
    void parse_dimensions(TextureRegion& tile, cJSON* sprite);
    void parse_indices(TextureRegion& tile, cJSON* sprite);
    void parse_positions(TextureRegion& tile, cJSON* sprite);
    void parse_uvs(TextureRegion& tile, cJSON* sprite);
}

TextureRegion TextureRegion::fromJson(std::size_t region_index, void* document_root)
{
    TextureRegion tile{};
    tile.region_index = region_index;

    cJSON* sprites_array = cJSON_GetObjectItemCaseSensitive(reinterpret_cast<cJSON*>(document_root), "sprites");
    assert (sprites_array && cJSON_IsArray(sprites_array));

    std::stringstream s;
    s << region_index;
    s << ".png";
    const std::string filename = s.str();

    // Iterate as long as there is any sprite left in the document, or found the one matching type:
    for (std::uint16_t index = 0; index < std::numeric_limits<uint16_t>::max(); index++)
    {
        cJSON* sprite_document = cJSON_GetArrayItem(sprites_array, index);
        if (!sprite_document)
        {
            break;
        }

        cJSON* sprite_filename_document = cJSON_GetObjectItemCaseSensitive(sprite_document, "name");
        assert(sprite_filename_document && cJSON_IsString(sprite_filename_document));

        const std::string image_filename = cJSON_GetStringValue(sprite_filename_document);

        if (filename == image_filename)
        {
            parse_dimensions(tile, sprite_document);
            parse_indices(tile, sprite_document);
            parse_positions(tile, sprite_document);
            parse_uvs(tile, sprite_document);
            tile.ok = true;
            return tile;
        }
    }
    return tile;
}

void TextureRegion::normalize(std::uint16_t spritesheet_width, std::uint16_t spritesheet_height)
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

void TextureRegion::push_uvs(std::vector<GLfloat> &out_uvs) const
{
    for(std::size_t x = 0; x < 4; x++)
    {
        out_uvs.push_back(uv_normalized[x][0]);
        out_uvs.push_back(uv_normalized[x][1]);
    }
}

void TextureRegion::push_positions(std::vector<int16_t > &out_positions, int x_offset, int y_offset) const
{
    for(std::size_t x = 0; x < 4; x++)
    {
        out_positions.push_back(positions_normalized[x][0] + x_offset);
        out_positions.push_back(positions_normalized[x][1] + y_offset);
    }
}

void TextureRegion::push_indices(std::vector<std::int16_t> &out_indices, std::size_t offset) const
{
    for(std::size_t x = 0; x < 6; x++)
    {
        out_indices.push_back(indices[x] + (offset * 4));
    }
}

namespace
{
    void parse_dimensions(TextureRegion &tile, cJSON* sprite)
    {
        cJSON* dimensions_array = cJSON_GetObjectItemCaseSensitive(sprite, "size");
        assert(dimensions_array && cJSON_IsArray(dimensions_array) && cJSON_GetArraySize(dimensions_array) == 2);

        tile.width = cJSON_GetArrayItem(dimensions_array, 0)->valueint;
        tile.height = cJSON_GetArrayItem(dimensions_array, 1)->valueint;
    }

    void parse_indices(TextureRegion &tile, cJSON* sprite)
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

    void parse_positions(TextureRegion &tile, cJSON* sprite)
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

    void parse_uvs(TextureRegion &tile, cJSON* sprite)
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
}
