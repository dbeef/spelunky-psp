#pragma once

#include <cstdint>

class Viewport final
{
public:

    Viewport(uint16_t width, uint16_t height)
        : _width(width)
        , _height(height)
        , _aspect(static_cast<float>(_width) / _height)
    { }

    float get_aspect() const { return _aspect; }

    uint16_t get_window_width() const { return _width; }

    uint16_t get_window_height() const { return _height; }

private:

    const uint16_t _width;
    const uint16_t _height;
    const float _aspect;
};
