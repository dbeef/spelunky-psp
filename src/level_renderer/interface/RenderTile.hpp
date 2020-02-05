//
// Created by dbeef on 2/5/20.
//

#ifndef RESOURCE_COMPILER_RENDERTILE_HPP
#define RESOURCE_COMPILER_RENDERTILE_HPP

#include "MapTileType.hpp"
#include "tao/json/value.hpp"

#include <cstdint>
#include <string>

struct RenderTile
{
    static RenderTile fromJson(MapTileType type, tao::json::value &document_root);

    MapTileType type;
    std::uint16_t width;
    std::uint16_t height;
    std::uint16_t uv[4][2];
    std::uint16_t postions[4][2];
    std::uint16_t indices[6];
};

#endif //RESOURCE_COMPILER_RENDERTILE_HPP
