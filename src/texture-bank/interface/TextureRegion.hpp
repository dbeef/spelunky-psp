//
// Created by dbeef on 2/5/20.
//

#ifndef RESOURCE_COMPILER_TEXTUREREGION_HPP
#define RESOURCE_COMPILER_TEXTUREREGION_HPP

#include "glad/glad.h"

#include <cstdint>
#include <string>
#include <vector>

struct TextureRegion
{
    static TextureRegion fromJson(std::size_t region_index, void* document_root);
    void normalize(std::uint16_t spritesheet_width, std::uint16_t spritesheet_height);

    void push_uvs(std::vector<GLfloat>& out_uvs) const;
    void push_positions(std::vector<int16_t>& out_positions, int x_offset, int y_offset) const;
    void push_indices(std::vector<std::int16_t>& out_indices, std::size_t offset) const;

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
