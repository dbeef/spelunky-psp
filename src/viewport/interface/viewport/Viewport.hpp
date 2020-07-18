#pragma once

#include <cstdint>
#include <cstdlib>

using WorldUnit_t = float;
using Pixel_t = uint16_t;

class Viewport final
{
public:

    Viewport(uint16_t width, uint16_t height)
        : _width(width)
        , _height(height)
        , _aspect(static_cast<float>(_width) / _height)
    { }

    float get_aspect() const { return _aspect; }

    Pixel_t get_width_pixels() const { return _width; }
    Pixel_t get_height_pixels() const { return _height; }

    WorldUnit_t get_width_world_units() const { return _SCREEN_WIDTH_IN_TILES; }
    WorldUnit_t get_height_world_units() const { return calculate_coefficient_world_units() * 2.0f;}

    WorldUnit_t world_units_to_pixels(Pixel_t pixels) const
    {
        // Tiles are used as the basis for pixel to world-unit conversion.
        // 1 world unit is equal to 1 tile width/height.
        return static_cast<float>(_width) / _SCREEN_WIDTH_IN_TILES;
    }

    // Returns coefficient to glOrtho of such value,
    // so screen with dimensions of screen_width_tiles would entirely fit on the screen.
    float calculate_coefficient_world_units() const { return (get_width_world_units() / get_aspect()) / 2.0f; }
    // Same but using pixels as the base camera unit.
    float calculate_coefficient_pixels() const { return (get_width_pixels() / get_aspect()) / 2.0f; }

private:

    const uint16_t _width;
    const uint16_t _height;
    const float _aspect;
    const std::size_t _SCREEN_WIDTH_IN_TILES = 20;
};
