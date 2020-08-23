#pragma once

#include "TextureBank.hpp"
#include "RenderingLayer.hpp"

#include <cstdint>

using RenderEntityID = uint16_t;

struct RenderEntity
{
    Vertex* vertices;
    IndexType* indices;
    uint32_t indices_count;
    TextureID texture;
    RenderingLayer layer;

    RenderEntityID id;
};
