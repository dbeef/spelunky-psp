#pragma once

#include <cstdint>

#include "Vertex.hpp"
#include "IndexType.hpp"
#include "TextureID.hpp"
#include "CameraType.hpp"
#include "RenderingLayer.hpp"

struct MeshComponent
{
    Vertex* vertices = nullptr;
    IndexType* indices = nullptr;
    std::size_t indices_count = 0;
    TextureID texture_id = 0;
    RenderingLayer rendering_layer;
    CameraType camera_type;
};
