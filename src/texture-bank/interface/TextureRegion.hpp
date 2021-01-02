//
// Created by dbeef on 2/5/20.
//

#ifndef RESOURCE_COMPILER_TEXTUREREGION_HPP
#define RESOURCE_COMPILER_TEXTUREREGION_HPP

#include "Mesh.hpp"
#include "Quad.hpp"

#include <cstdint>
#include <string>
#include <vector>

struct TextureRegion
{
    static TextureRegion fromJson(std::size_t region_index, void* document_root);
    void normalize(std::uint16_t spritesheet_width, std::uint16_t spritesheet_height);

    void set_quad_uv(Quad&, bool vflip = false, bool hflip = false) const;
    void set_quad_xy(Quad&) const;
    void set_quad_indices(Quad&, uint16_t offset = 0) const;

    void set_quad_uv(Vertex*, bool vflip = false, bool hflip = false) const;
    void set_quad_xy(Vertex*) const;
    void set_quad_indices(IndexType*, uint16_t offset = 0) const;

    std::vector<Vertex> get_quad_vertices(float x, float y, bool vflip = false, bool hflip = false) const;
    std::vector<IndexType> get_quad_indices(uint16_t offset = 0) const;

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
