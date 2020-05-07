//
// Created by dbeef on 2/5/20.
//

#ifndef RESOURCE_COMPILER_TEXTUREREGION_HPP
#define RESOURCE_COMPILER_TEXTUREREGION_HPP

#include "glad/glad.h"

#include <cstdint>
#include <string>
#include <vector>

using IndicesType = int16_t;

struct Vertex
{
    float x;
    float y;
    float u;
    float v;
};

struct TextureRegion
{
    static TextureRegion fromJson(std::size_t region_index, void* document_root);
    void normalize(std::uint16_t spritesheet_width, std::uint16_t spritesheet_height);

    std::vector<Vertex> get_quad_mesh(float x, float y) const;
    std::vector<IndicesType> get_quad_indices(uint16_t offset = 0) const;

    std::size_t region_index;
    std::uint16_t width;
    std::uint16_t height;

    std::uint16_t uv[4][2];
    std::uint16_t positions[4][2];

    float uv_normalized[4][2];
    float positions_normalized[4][2];

    std::uint16_t indices[6];

    bool ok = false;
};

#endif //RESOURCE_COMPILER_TEXTUREREGION_HPP
