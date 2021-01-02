#pragma once

#include "MapTile.hpp"

struct PositionComponent
{
    PositionComponent(float x_center, float y_center) : x_center(x_center), y_center(y_center) {}
    PositionComponent() = default;

    void set_position_on_tile(const MapTile* tile)
    {
        x_center = tile->x + MapTile::PHYSICAL_WIDTH / 2;
        y_center = tile->y + MapTile::PHYSICAL_HEIGHT / 2;
    }

    float x_center = 0;
    float y_center = 0;
};
