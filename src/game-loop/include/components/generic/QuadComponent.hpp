#pragma once

#include "IndexType.hpp"
#include "Vertex.hpp"
#include "RenderingLayer.hpp"

#include <entt/entt.hpp>

#include "RenderEntity.hpp"
#include "TextureType.hpp"
#include "Quad.hpp"
#include "CameraType.hpp"

#include <cstdint>

class QuadComponent
{
public:

    QuadComponent(TextureType, float quad_width, float quad_height);

    void update(entt::entity owner);

    template <class T>
    void frame_changed(T frame_index) { _frame_index = static_cast<std::size_t>(frame_index);_frame_changed = true; }
    void frame_changed() { _frame_changed = true; }

    template <class T>
    T get_current_frame() const { return static_cast<T>(_frame_index); }

    float get_quad_width() const { return _quad_dimensions.width; }
    float get_quad_height() const { return _quad_dimensions.height; }

    void set_quad_width(float width) { _quad_dimensions.width = width; }
    void set_quad_height(float height) { _quad_dimensions.height = height; }

    Quad& get_quad() { return _quad; }

    void set_vertical_flip(bool v) { _vertical_flip = v; }
    bool is_vertically_flipped() const { return _vertical_flip; }

private:

    struct
    {
        float width = 1.0;
        float height = 1.0;
    } _quad_dimensions;

    std::size_t _frame_index = 0;
    bool _frame_changed = true;
    bool _vertical_flip = false;

    Quad _quad;
    TextureType _texture_type;
};
