//
// Created by dbeef on 2/5/20.
//

#include "TextureRegion.hpp"
#include "logger/log.h"
#include "cJSON.h"

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

std::vector<Vertex> TextureRegion::get_quad_vertices(float x, float y, bool vflip, bool hflip) const
{
    std::vector<Vertex> out;

    for (std::size_t index = 0; index < 4; index++)
    {
        Vertex v;
        v.x = positions_normalized[index][0] + x;
        v.y = positions_normalized[index][1] + y;

        if (hflip)
        {
            v.u = uv_normalized[3 - index][0];
            v.v = uv_normalized[3 - index][1];
        }
        else if (vflip)
        {
            if (index % 2)
            {
                v.u = uv_normalized[index - 1][0];
                v.v = uv_normalized[index - 1][1];
            }
            else
            {
                v.u = uv_normalized[index + 1][0];
                v.v = uv_normalized[index + 1][1];
            }
        }
        else
        {
            v.u = uv_normalized[index][0];
            v.v = uv_normalized[index][1];
        }

        out.push_back(v);
    }
    return out;
}

std::vector<IndexType> TextureRegion::get_quad_indices(uint16_t offset) const
{
    std::vector<IndexType> out;
    for(unsigned short i : indices)
    {
        out.push_back(i + (offset * 4));
    }
    return out;
}

void TextureRegion::set_quad_uv(Vertex* vertices_base, bool vflip, bool hflip) const
{
    for (std::size_t index = 0; index < 4; index++)
    {
        Vertex* v = vertices_base + index;

        if (hflip)
        {
            v->u = uv_normalized[3 - index][0];
            v->v = uv_normalized[3 - index][1];
        }
        else if (vflip)
        {
            if (index % 2)
            {
                v->u = uv_normalized[index - 1][0];
                v->v = uv_normalized[index - 1][1];
            }
            else
            {
                v->u = uv_normalized[index + 1][0];
                v->v = uv_normalized[index + 1][1];
            }
        }
        else
        {
            v->u = uv_normalized[index][0];
            v->v = uv_normalized[index][1];
        }
    }
}

void TextureRegion::set_quad_uv(Quad& quad, bool vflip, bool hflip) const
{
    Vertex* out = quad.get_vertices_base();

    for (std::size_t index = 0; index < 4; index++)
    {
        Vertex* v = out + index;

        if (hflip)
        {
            v->u = uv_normalized[3 - index][0];
            v->v = uv_normalized[3 - index][1];
        }
        else if (vflip)
        {
            if (index % 2)
            {
                v->u = uv_normalized[index - 1][0];
                v->v = uv_normalized[index - 1][1];
            }
            else
            {
                v->u = uv_normalized[index + 1][0];
                v->v = uv_normalized[index + 1][1];
            }
        }
        else
        {
            v->u = uv_normalized[index][0];
            v->v = uv_normalized[index][1];
        }
    }
}

void TextureRegion::set_quad_indices(IndexType* out_indices, uint16_t offset) const
{
    for (std::size_t index = 0; index < 6; index++)
    {
        auto value = indices[index];
        out_indices[index] = (value + (offset * 4));
    }
}

void TextureRegion::set_quad_indices(Quad& quad, uint16_t offset) const
{
    IndexType* out = quad.get_indices();

    for (std::size_t index = 0; index < 6; index++)
    {
        auto value = indices[index];
        out[index] = (value + (offset * 4));
    }
}

void TextureRegion::set_quad_xy(Vertex* vertices_base) const
{
    for (std::size_t index = 0; index < 4; index++)
    {
        Vertex *v = vertices_base + index;

        v->x = positions_normalized[index][0];
        v->y = positions_normalized[index][1];
    }
}

void TextureRegion::set_quad_xy(Quad& quad) const
{
    Vertex* out = quad.get_vertices_base();

    for (std::size_t index = 0; index < 4; index++)
    {
        Vertex *v = out + index;

        v->x = positions_normalized[index][0];
        v->y = positions_normalized[index][1];
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
