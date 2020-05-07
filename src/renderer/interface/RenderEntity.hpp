#pragma once

#include "TextureBank.hpp"
#include <cstdint>

using RenderEntityID = uint16_t;
using IndicesType = int16_t;

struct Vertex
{
    int16_t x;
    int16_t y;
    float u;
    float v;
};

struct RenderEntity
{
    Vertex* vertices;
    IndicesType* indices;
    uint32_t indices_count;
    TextureID texture;

    RenderEntityID id;
};
