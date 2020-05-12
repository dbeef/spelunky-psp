#pragma once

#include "TextureBank.hpp"
#include <cstdint>

using RenderEntityID = uint16_t;

struct RenderEntity
{
    Vertex* vertices;
    IndexType* indices;
    uint32_t indices_count;
    TextureID texture;

    RenderEntityID id;
};
