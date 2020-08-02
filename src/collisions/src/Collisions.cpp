#include "Collisions.hpp"
#include "NeighbouringTiles.hpp"

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

MapTile* collisions::overlaps_strict(MapTile **neighboring_tiles, float x_center, float y_center, float width, float height, bool collidable)
{
    const float half_w = width / 2.0f;
    const float half_h = height / 2.0f;

    for (int i = 0; i < static_cast<int>(NeighbouringTiles::_SIZE); i++) {

        if (neighboring_tiles[i] == nullptr)
        {
            continue;
        }

        if (neighboring_tiles[i]->collidable != collidable)
        {
            continue;
        }

        if (x_center + half_w >= neighboring_tiles[i]->x && x_center - half_w <= neighboring_tiles[i]->x + MapTile::PHYSICAL_WIDTH &&
            y_center + half_h >= neighboring_tiles[i]->y && y_center - half_h <= neighboring_tiles[i]->y + MapTile::PHYSICAL_HEIGHT)
        {
            return neighboring_tiles[i];
        }
    }

    return nullptr;
}

MapTile* collisions::overlaps(MapTile **neighboring_tiles, float x_center, float y_center, float width, float height, bool collidable)
{
    for (int i = 0; i < static_cast<int>(NeighbouringTiles::_SIZE); i++)
    {

        if (neighboring_tiles[i] == nullptr)
        {
            continue;
        }

        if (neighboring_tiles[i]->collidable != collidable)
        {
            continue;
        }

        if (overlaps(neighboring_tiles[i], x_center, y_center, width, height))
        {
            return neighboring_tiles[i];
        }
    }

    return nullptr;
}

bool collisions::overlaps(MapTile *tile, float x_center, float y_center, float width, float height)
{
    const float half_w = width / 2.0f;
    const float half_h = height / 2.0f;

    return x_center + half_w > tile->x && x_center - half_w < tile->x + MapTile::PHYSICAL_WIDTH &&
           y_center + half_h > tile->y && y_center - half_h < tile->y + MapTile::PHYSICAL_HEIGHT;
}
