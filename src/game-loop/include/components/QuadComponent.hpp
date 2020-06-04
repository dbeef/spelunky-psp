#pragma once

#include <cstdint>

#include "Renderer.hpp"
#include "RenderEntity.hpp"
#include "TextureType.hpp"
#include "Quad.hpp"
#include "MapTile.hpp"

class QuadComponent
{
public:

    QuadComponent(TextureType, Renderer::EntityType, float quad_width, float quad_height);
    QuadComponent(const QuadComponent&);
    ~QuadComponent();

    void update(float pos_x_center, float pos_y_center, bool vertical_flip = false);

    template <class T>
    void frame_changed(T frame_index) { _frame_index = static_cast<std::size_t>(frame_index);_frame_changed = true; }
    void frame_changed() { _frame_changed = true; }

    template <class T>
    T get_current_frame() const { return static_cast<T>(_frame_index); }

    float get_quad_width() const { return _quad_dimensions.width; }
    float get_quad_height() const { return _quad_dimensions.height; }

private:

    const struct
    {
        float width;
        float height;
    } _quad_dimensions;

    std::size_t _frame_index = 0;
    bool _frame_changed = true;

    Quad _quad;
    RenderEntityID _render_entity_id;
    const TextureType _texture_type;
    const Renderer::EntityType _entity_type;
};
