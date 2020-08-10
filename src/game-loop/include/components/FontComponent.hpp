#pragma once

#include <string>

#include "viewport/Viewport.hpp"
#include "Point2D.hpp"
#include "QuadComponent.hpp"


class FontComponent
{
public:

    void set_text(const std::string& text) { _properties.text = text; update(); }
    void set_yellow(bool yellow) { _properties.yellow = yellow; update(); }
    void set_scale(float scale) { _properties.scale = scale; update(); }
    void set_position(Point2D pos_screen_space) { _properties.pos_screen_space = pos_screen_space; update(); }

    WorldUnit_t get_font_width() { return 0.5f * _properties.scale; }
    WorldUnit_t get_font_height() { return 0.5f * _properties.scale; }

private:

    void update();

    struct
    {
        bool yellow = false;
        float scale = 1.0f;
        Point2D pos_screen_space{0, 0};
        std::string text;
    } _properties;

    std::vector<QuadComponent> _quads;
};
