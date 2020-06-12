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

    uint16_t get_width() const { return _width; }
    uint16_t get_height() const { return _height; }
    float get_aspect() const { return _aspect; }

private:

    const uint16_t _width;
    const uint16_t _height;
    const float _aspect;
};
