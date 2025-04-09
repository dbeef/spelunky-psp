#pragma once

#include <string>

#include "viewport/Viewport.hpp"
#include "Point2D.hpp"
#include "QuadComponent.hpp"

#include "entt/entt.hpp"

class TextComponent
{
public:

    void set_text(const std::string& text) { _properties.text = text; _dirty = true; }
    void set_yellow(bool yellow) { _properties.yellow = yellow; _dirty = true; }
    void set_scale(float scale) { _properties.scale = scale; _dirty = true; }
    void set_layer(RenderingLayer layer) { _properties.layer = layer; _dirty = true; };

    WorldUnit_t get_font_width() const { return 0.5f * _properties.scale; }
    WorldUnit_t get_font_height() const { return 0.5f * _properties.scale; }
    WorldUnit_t get_width() const { return _properties.text.size() * get_font_width(); }

    void update(entt::entity owner);

private:

    bool _dirty = false;

    struct
    {
        bool yellow = false;
        float scale = 1.0f;
        std::string text;
        RenderingLayer layer = RenderingLayer::LAYER_2_HUD;
    } _properties;

    std::vector<Vertex> _mesh;
    std::vector<IndexType> _indices;
};
