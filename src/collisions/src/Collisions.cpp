#include "Collisions.hpp"

#include <cstdint>
#include <cmath>

// XY axes go as following:
// ____ +x
// |
// |
// +y
//
// Rendering dimensions are same as physical dimensions, in other words,
// a quad made of 1x1 vertices will have a collision box of 1x1.

MapTile *collisions::overlaps_strict(MapTile **neighboring_tiles, float x_center, float y_center, float width, float height, bool collidable)
{
    bool condition_x;
    bool condition_y;

    const float half_w = width / 2;
    const float half_h = height / 2;

    const float tile_w = 1.0f;
    const float tile_h = 1.0f;

    for (int a = 0; a < 9; a++) {

        if (neighboring_tiles[a] == nullptr)
        {
            continue;
        }

        if (neighboring_tiles[a]->collidable != collidable)
        {
            continue;
        }

        condition_x = x_center + half_w >= neighboring_tiles[a]->x && x_center - half_w <= neighboring_tiles[a]->x + tile_w;
        condition_y = y_center + half_h >= neighboring_tiles[a]->y && y_center - half_h <= neighboring_tiles[a]->y + tile_h;

        if (condition_x && condition_y)
        {
            return neighboring_tiles[a];
        }
    }

    return nullptr;
}

MapTile *collisions::overlaps(MapTile **neighboring_tiles, float x_center, float y_center, float width, float height, bool collidable)
{
    for (int a = 0; a < 9; a++)
    {

        if (neighboring_tiles[a] == nullptr)
        {
            continue;
        }

        if (neighboring_tiles[a]->collidable != collidable)
        {
            continue;
        }

        if (overlaps(neighboring_tiles[a], x_center, y_center, width, height))
        {
            return neighboring_tiles[a];
        }
    }

    return nullptr;
}

bool collisions::overlaps(MapTile *tile, float x_center, float y_center, float width, float height)
{
    bool condition_x;
    bool condition_y;

    const float half_w = width / 2;
    const float half_h = height / 2;

    const float tile_w = 1.0f;
    const float tile_h = 1.0f;

    condition_x = x_center + half_w > tile->x && x_center - half_w < tile->x + tile_w;
    condition_y = y_center + half_h > tile->y && y_center - half_h < tile->y + tile_h;

    return condition_x && condition_y;
}
