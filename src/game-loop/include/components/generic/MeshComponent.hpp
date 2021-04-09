#pragma once

#include <cstdint>

#include "Vertex.hpp"
#include "IndexType.hpp"
#include "TextureID.hpp"
#include "CameraType.hpp"
#include "RenderingLayer.hpp"

struct MeshComponent
{
    MeshComponent() = default;
    MeshComponent(RenderingLayer rendering_layer, CameraType camera_type)
        : rendering_layer(rendering_layer)
        , camera_type(camera_type)
    {}

    Vertex* vertices = nullptr;
    IndexType* indices = nullptr;
    std::size_t indices_count = 0;
    TextureID texture_id = 0;
    RenderingLayer rendering_layer = RenderingLayer::LAYER_5_PROPS;
    CameraType camera_type = CameraType::SCREEN_SPACE;
};
