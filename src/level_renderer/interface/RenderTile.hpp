//
// Created by dbeef on 2/5/20.
//

#ifndef RESOURCE_COMPILER_RENDERTILE_HPP
#define RESOURCE_COMPILER_RENDERTILE_HPP

#include <cstdint>

#include "MapTileType.hpp"

struct RenderTile
{
    MapTileType type;
    std::uint16_t width;
    std::uint16_t height;
    std::uint16_t uv[4][2];
    std::uint16_t postions[4][2];
    std::uint16_t indices[6];
    const char* name;
};

#endif //RESOURCE_COMPILER_RENDERTILE_HPP
